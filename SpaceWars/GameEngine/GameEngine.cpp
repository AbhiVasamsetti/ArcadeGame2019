#include "GameEngine.h"

// constructor and destructor not defined
GameEngine::GameEngine()
{
    
}

GameEngine::~GameEngine()
{

}

// Finite State Machine (FSM) for enemy ship
StateEnemy fsm_enemy[4] = {
    {0,{1,0,0}},
    {1,{1,2,1}},  
    {2,{2,2,3}},
    {3,{0,0,0}}   
    };

// Finite State Machine (FSM) for bolt
StateBolt fsm_bolt[5] = {
    {0,{1,0,0}},  
    {1,{1,2,1}},
    {2,{2,3,2}}, 
    {3,{3,4,3}},
    {4,{4,4,0}} 
    };
    
// Initialises the game engine  
void GameEngine::init(int difficulty, N5110 &lcd, FXOS8700CQ &acc, Gamepad &pad, int sensitivity)
{
    _speed = difficulty; // the faster the ships move, the harder it is to avoid them, hence difficulty controls the speed variable
    _sensitivity = sensitivity; // sensitivity of the joystick
        
    _s1x = GAP; // Sets initial x position of the spaceship
    _s1y = 16; // Sets initial y position of the spaceship
        
    // Uses accelerometer to seed the rand function
    acc.init(); // initialises accelerometer
    Data values = acc.get_values(); // polls the sensor and stores the values in the Data struct        
    srand (values.ay*500); // seeds the rand function with a value from the Data struct multiplied by a large number
        
    // Assigns a random number to the y positions of the enemy ships
    _es1y = rand() % 32 + 3; // generates a random number between 3 and 32 and assigns this value to the y variable
    _es2y = rand() % 32 + 3; // generates a random number between 3 and 32 and assigns this value to the y variable
    _es3y = rand() % 32 + 3; // generates a random number between 3 and 32 and assigns this value to the y variable
        
    //Initialises spaceship
    _s1.init(_s1x,_s1y,_sensitivity); 
        
    // Initialises the 3 enemy ships, 20 pixels apart from each other off screen
    _es1.init(WIDTH,_es1y,_speed); // The first ship to appear in the game
    _es2.init(WIDTH+20,_es2y,_speed);
    _es3.init(WIDTH+40,_es3y,_speed);
        
    // Initialises all the bolts at the cannon side of the spaceship
    _bolt1.init(_s1x+3,_s1y+4,7,2,_sensitivity);
    _bolt2.init(_s1x+3,_s1y+4,7,2,_sensitivity);
    _bolt3.init(_s1x+3,_s1y+4,7,2,_sensitivity);
    
    // Turn all the leds on
    pad.leds_on();
        
    // Set initial output states of FSMs to 0
    state_bolt = 0;
    state_enemy = 0;
        
    // Set initial input states of FSMs to 2
    input_bolt = 2;
    input_enemy = 2;
        
    // Set initial score to 0;
    _score = 0;
    
    // Set initial lives to 3;
    _lives = 3;
}

// Runs the game loop
void GameEngine::run_engine(Gamepad &pad,N5110 &lcd, FXOS8700CQ &acc)
{
    read_input(pad);
    draw(lcd); 
    output_object();
    run_output(acc);
    health_bar(pad);
    ammo_bar(pad);
    check_all_collisions(pad,acc); 
    state_object();
}

// Reads the input functions and the gamepad
void GameEngine::read_input(Gamepad &pad)
{
    // Reads the joystick direction and magnitude of the movement
    _d = pad.get_direction();
    _mag = pad.get_mag();
    
    // Collects the input functions for the FSMs
    input_ships(pad);
    input_bolts(pad.check_event(Gamepad::L_PRESSED),
                pad.check_event(Gamepad::R_PRESSED),pad);
                
    //printf("input enemy = %d\n",input_enemy);
    //printf("input bolt = %d\n",input_bolt);
}

// Sets the output state of the FSM based on the current state of the bolt and enemy FSMs
void GameEngine::output_object()
{
    output_enemy = fsm_enemy[state_enemy].output;
    output_bolt = fsm_bolt[state_bolt].output; 
    
    //printf("output enemy = %d\n",output_enemy);
    //printf("output bolt = %d\n",output_bolt);
}

// Runs the output functions of the bolt and enemy FSMs and updates the spaceship
void GameEngine::run_output(FXOS8700CQ &acc)
{
    _s1.update(_d,_mag); 
    output_ships(acc);   
    output_bolts();     
}

// Renders all the objects onto the lcd screen
void GameEngine::draw(N5110 &lcd)
{
    _s1.draw(lcd); // renders the spaceship

    // Renders all the enemy ships
    _es1.draw(lcd);
    _es2.draw(lcd);
    _es3.draw(lcd);
    
    // Renders all the bolts
    _bolt1.draw(lcd);
    _bolt2.draw(lcd);
    _bolt3.draw(lcd);
}

// Checks to see if enemy ships pass off screen and change the input of the enemy FSM
void GameEngine::input_ships(Gamepad &pad)
{
    Vector2D enemyship_pos1 = _es1.get_pos(); // Gets the x and y positions of enemy ship 1
    Vector2D enemyship_pos2 = _es2.get_pos(); // Gets the x and y positions of enemy ship 2
    Vector2D enemyship_pos3 = _es3.get_pos(); // Gets the x and y positions of enemy ship 3

    if (enemyship_pos1.x+15 <= 0) {
        input_enemy = 0; //If ship 1 passes off screen
    } 
    if (enemyship_pos2.x+15 <= 0) {
        input_enemy = 1; // If ship 2 passes off screen
    } 
    if (enemyship_pos3.x+15 <= 0) {
        input_enemy = 2; // If ship 3 passes off screen
    }   
}

// Reads the input of the R and L buttons and changes the input of the bolt FSM
void GameEngine::input_bolts(bool l, bool r, Gamepad &pad)
{   
    if (!l && !r){ 
        input_bolt = 0; // if no button is pressed
    } else if (l){
        input_bolt = 1; // if L is pressed
        pad.tone(400,0.1); // sound for the L button
    } else if (r){
        input_bolt = 2; // if R is pressed
        pad.tone(2500,0.2); // sound for the R button
        pad.tone(2500,0.2);
    }
}

// Sets the next state of the bolt and enemy FSMs based on the input and current state of the FSMs.
void GameEngine::state_object()
{
    state_enemy = fsm_enemy[state_enemy].nextState[input_enemy];
    state_bolt = fsm_bolt[state_bolt].nextState[input_bolt];
}

// Collects all the collision functions
void GameEngine::check_all_collisions(Gamepad &pad, FXOS8700CQ &acc)
{
    check_spaceship_collisions(pad,acc);
    check_bolt_collisions(acc,pad);
}

// Collects all the spaceship collision functions
void GameEngine::check_spaceship_collisions(Gamepad &pad, FXOS8700CQ &acc)
{
    check_spaceship_collisions_one(pad,acc);
    check_spaceship_collisions_two(pad,acc);
    check_spaceship_collisions_three(pad,acc);
}

// Collects all the bolt collisions with all the ships
void GameEngine::check_bolt_collisions(FXOS8700CQ &acc,Gamepad &pad)
{
    check_bolt_collision_ship_one(acc,pad);
    check_bolt_collision_ship_two(acc,pad);
    check_bolt_collision_ship_three(acc,pad);
}

// Collects all the bolt collision functions with ship 1
void GameEngine::check_bolt_collision_ship_one(FXOS8700CQ &acc,Gamepad &pad)
{
    check_bolt_one_collision_ship_one(acc,pad);
    check_bolt_two_collision_ship_one(acc,pad);
    check_bolt_three_collision_ship_one(acc,pad);
}

// Collects all the bolt collision functions with ship 2
void GameEngine::check_bolt_collision_ship_two(FXOS8700CQ &acc,Gamepad &pad)
{
    check_bolt_one_collision_ship_two(acc,pad);
    check_bolt_two_collision_ship_two(acc,pad);
    check_bolt_three_collision_ship_two(acc,pad);
}

// Collects all the bolt collision functions with ship 3
void GameEngine::check_bolt_collision_ship_three(FXOS8700CQ &acc,Gamepad &pad)
{
    check_bolt_one_collision_ship_three(acc,pad);
    check_bolt_two_collision_ship_three(acc,pad);
    check_bolt_three_collision_ship_three(acc,pad);
}


// Takes the output state of the enemy FSM and outputs corresponding actions
void GameEngine::output_ships(FXOS8700CQ &acc)
{           
    if (output_enemy == 0){ 
        _es1.update(); // Updates ship 1 from the far side of the screen
    } else if (output_enemy == 1){
        // Updates ships 1 and 2
        _es1.update(); // Unless ship 1 collides, it needs to keep updating, hence the ship 1 update function
        _es2.update();
    } else if (output_enemy == 2){
        // Updates all the ships
        _es1.update(); // Unless there are any collsions, ships 1 to 3 will continue to keep updating
        _es2.update();
        _es3.update();
    } else if (output_enemy == 3){
        Data values = acc.get_values(); // polls the sensor and stores the values in the Data struct        
        srand (values.ay*500); // seeds the rand function with a value from the Data struct multiplied by a large number
        _es1y = rand() % 32 + 3; // generates a random number between 3 and 32 and assigns this value to the y variable
        _es2y = rand() % 32 + 3; // generates a random number between 3 and 32 and assigns this value to the y variable
        _es3y = rand() % 32 + 3; // generates a random number between 3 and 32 and assigns this value to the y variable
        
        // Initialises all the enemy ships at the far side off screen, and 40 pixels apart 
        _es1.init(WIDTH,_es1y,_speed);
        _es2.init(WIDTH+40,_es2y,_speed);
        _es3.init(WIDTH+80,_es3y,_speed);
    } 
}

// Takes the output state of the bolt FSM and outputs corresponding actions
void GameEngine::output_bolts()
{
    Vector2D spaceship_pos = _s1.get_pos(); // Gets the x and y positions of spaceship
    
    
    if (output_bolt == 0){
        // Initialises all bolts in the cannon area of the spaceship specified by the set x and y positions
        _bolt1.init(spaceship_pos.x+3,spaceship_pos.y+4,6,2,_sensitivity);
        _bolt2.init(spaceship_pos.x+3,spaceship_pos.y+4,6,2,_sensitivity);
        _bolt3.init(spaceship_pos.x+3,spaceship_pos.y+4,6,2,_sensitivity); 
               
        _ammo = 3; // Sets the ammo to 3
    } 
    if (output_bolt == 1){
        // Updates all the bolts in the y direction using the joystick
        _bolt1.initial_update_y(_d,_mag);
        _bolt2.initial_update_y(_d,_mag);
        _bolt3.initial_update_y(_d,_mag);
    } 
    if (output_bolt == 2){   
        // Fires bolt 1  
        _bolt1.update_x(); // fixes y position of bolt when fired, hence why bolt only updates in x direction
        _bolt2.initial_update_y(_d,_mag); // bolts 2 and 3 are not fired yet so they need to still update in y direction
        _bolt3.initial_update_y(_d,_mag);
        
        _ammo = 2; // Sets the ammo to 2
    } 
    if (output_bolt == 3){
        // Fires bolt 2
        _bolt1.update_x(); // If bolt 1 has not collided, it must keep moving, hence why both bolt 1 and 2 update in x direction
        _bolt2.update_x(); // bolt 2 updates in x direction and fixes y position
        _bolt3.initial_update_y(_d,_mag); // bolt 3 not fired so keeps updating in the y direction
        
        _ammo = 1; // Sets the ammo to 1
    } 
    if (output_bolt == 4){
        // Fires bolt 3
        _bolt1.update_x(); 
        _bolt2.update_x();
        _bolt3.update_x(); // bolt 3 fired, so y position fixed and updates in x direction
        
        _ammo = 0; // Sets the ammo to 0
    } 
}

// Checks the collision between spaceship and enemy ship 1
void GameEngine::check_spaceship_collisions_one(Gamepad &pad, FXOS8700CQ &acc) 
{
    Vector2D enemyship_pos1 = _es1.get_pos(); // Gets the x and y positions of enemy ship 1
    Vector2D enemyship_pos3 = _es3.get_pos(); // Gets the x and y positions of enemy ship 3
    Vector2D spaceship_pos = _s1.get_pos(); // Gets the x and y positions of spaceship
    
    /* Checks if enemy ship hits spaceship by checking overlaps
    * NB - Since enemy ship initialises again when it passes the 
    *      spaceship side of the screen, there is no overlap condition 
    *      on the left side of the spaceship)
    */ 
    if (
    (enemyship_pos1.x <= spaceship_pos.x+17) && // right side of spaceship
    (enemyship_pos1.y <= spaceship_pos.y+9) && // bottom side of spaceship
    (enemyship_pos1.y+5 >=spaceship_pos.y) && // top side of spaceship
    (enemyship_pos1.x+14 >= spaceship_pos.x) // left side of spaceship
    ) {
        // When enemy ship hits spaceship, the following actions occur
        Data values = acc.get_values(); // polls the sensor and stores the values in the Data struct        
        srand (values.ay*500); // seeds the rand function with a value from the Data struct multiplied by a large number
        _es1y = rand() % 32 + 3; // generates a random number between 3 and 32 and assigns this value to the y variable 
        _es1.init(enemyship_pos3.x+40,_es1y,_speed);  // Initialises ship 1 40 pixels behind ship 3 in the x direction off screen 
        
        _lives--; // decreases the lives by 1  
        
        pad.tone(800,0.3); // plays a sound when collision occurs      
    }
}

// Checks the collision between spaceship and enemy ship 2
void GameEngine::check_spaceship_collisions_two(Gamepad &pad, FXOS8700CQ &acc) 
{

    Vector2D enemyship_pos1 = _es1.get_pos(); // Gets the x and y positions of enemy ship 1
    Vector2D enemyship_pos2 = _es2.get_pos(); // Gets the x and y positions of enemy ship 2
    Vector2D spaceship_pos = _s1.get_pos(); // Gets the x and y positions of spaceship
    
    /* Checks if enemy ship hits spaceship by checking overlaps
    * NB - Since enemy ship initialises again when it passes the 
    *      spaceship side of the screen, there is no overlap condition 
    *      on the left side of the spaceship)
    */ 
    if (
    (enemyship_pos2.x <= spaceship_pos.x+17) && // right side of spaceship
    (enemyship_pos2.y <= spaceship_pos.y+9) && // bottom side of spaceship
    (enemyship_pos2.y+5 >=spaceship_pos.y) && // top side of spaceship
    (enemyship_pos2.x+14 >= spaceship_pos.x) // left side of spaceship
    ) {
        // When enemy ship hits spaceship, the following actions occur
        Data values = acc.get_values(); // polls the sensor and stores the values in the Data struct        
        srand (values.ay*500); // seeds the rand function with a value from the Data struct multiplied by a large number
        _es2y = rand() % 32 + 3; // generates a random number between 3 and 32 and assigns this value to the y variable 
        _es2.init(enemyship_pos1.x+40,_es3y,_speed); // Initialises ship 2 40 pixels behind ship 1 in the x direction off screen 
        
        _lives--; // decreases the lives by 1
        
        pad.tone(800,0.3); // plays a sound when collision occurs
    }
}

// Checks the collision between spaceship and enemy ship 3
void GameEngine::check_spaceship_collisions_three(Gamepad &pad, FXOS8700CQ &acc) 
{
    Vector2D enemyship_pos2 = _es2.get_pos(); // Gets the x and y positions of enemy ship 2
    Vector2D enemyship_pos3 = _es3.get_pos(); // Gets the x and y positions of enemy ship 3
    Vector2D spaceship_pos = _s1.get_pos(); // Gets the x and y positions of spaceship
    
    /* Checks if enemy ship hits spaceship by checking overlaps
    * NB - Since enemy ship initialises again when it passes the 
    *      spaceship side of the screen, there is no overlap condition 
    *      on the left side of the spaceship)
    */ 
    if (
    (enemyship_pos3.x <= spaceship_pos.x+17) && // right side of spaceship
    (enemyship_pos3.y <= spaceship_pos.y+9) && // bottom side of spaceship
    (enemyship_pos3.y+5 >=spaceship_pos.y) && // top side of spaceship
    (enemyship_pos3.x+14 >= spaceship_pos.x) // left side of spaceship
    ) {
        // When enemy ship hits spaceship, the following actions occur
        Data values = acc.get_values(); // polls the sensor and stores the values in the Data struct        
        srand (values.ay*500); // seeds the rand function with a value from the Data struct multiplied by a large number
        _es3y = rand() % 32 + 3; // generates a random number between 3 and 32 and assigns this value to the y variable 
        _es3.init(enemyship_pos2.x+40,_es3y,_speed); // Initialises ship 3 40 pixels behind ship 2 in the x direction off screen 
        
        _lives--; // decreases the lives by 1
        
        pad.tone(800,0.3); // plays a sound when collision occurs
    }
}

// Checks the collision between bolt 1 and enemy ship 1
void GameEngine::check_bolt_one_collision_ship_one(FXOS8700CQ &acc, Gamepad &pad)
{
    Vector2D bolt1_pos = _bolt1.get_pos(); // Gets the x and y positions of bolt 1
    Vector2D enemyship_pos1 = _es1.get_pos(); // Gets the x and y positions of enemy ship 1
    Vector2D enemyship_pos3 = _es3.get_pos(); // Gets the x and y positions of enemy ship 3
    
    // Checks if bolt hits enemy ship by checking overlaps
    if (
    (bolt1_pos.x+7 >= enemyship_pos1.x-1) && // right side of bolt
    (bolt1_pos.x <= enemyship_pos1.x+13) && // left side of bolt
    (bolt1_pos.y+1 >= enemyship_pos1.y) && // bottom side of bolt
    (bolt1_pos.y <= enemyship_pos1.y+6) // top side of bolt
    ) {
        // When bolt hits enemy ship, the following actions occur
        Data values = acc.get_values(); // polls the sensor and stores the values in the Data struct        
        srand (values.ay*500); // seeds the rand function with a value from the Data struct multiplied by a large number
        _es1y = rand() % 32 + 3; // generates a random number between 3 and 32 and assigns this value to the y variable 
        _es1.init(enemyship_pos3.x+40,_es1y,_speed);  // Initialises ship 1 40 pixels behind ship 3 in the x direction off screen      
        
        _bolt1.clear_bolt(); // clears bolt off screen
        
        _score += 1; // adds 1 to the score
        
        pad.tone(800,0.3); // plays a sound when bolt collides
        pad.tone(800,0.3);
    }   
}

// Checks the collision between bolt 2 and enemy ship 1
void GameEngine::check_bolt_two_collision_ship_one(FXOS8700CQ &acc,Gamepad &pad)
{
    Vector2D bolt2_pos = _bolt2.get_pos(); // Gets the x and y positions of bolt 2
    Vector2D enemyship_pos1 = _es1.get_pos(); // Gets the x and y positions of enemy ship 1
    Vector2D enemyship_pos3 = _es3.get_pos(); // Gets the x and y positions of enemy ship 3
    
    // Checks if bolt hits enemy ship by checking overlaps
    if (
    (bolt2_pos.x+7 >= enemyship_pos1.x-1) && // right side of bolt
    (bolt2_pos.x <= enemyship_pos1.x+13) && // left side of bolt
    (bolt2_pos.y+1 >= enemyship_pos1.y) && // bottom side of bolt
    (bolt2_pos.y <= enemyship_pos1.y+6) // top side of bolt
    ) {
        // When bolt hits enemy ship, the following actions occur
        Data values = acc.get_values(); // polls the sensor and stores the values in the Data struct        
        srand (values.ay*500); // seeds the rand function with a value from the Data struct multiplied by a large number
        _es1y = rand() % 32 + 3; // generates a random number between 3 and 32 and assigns this value to the y variable 
        _es1.init(enemyship_pos3.x+40,_es1y,_speed);  // Initialises ship 1 40 pixels behind ship 3 in the x direction off screen
        
        _bolt2.clear_bolt(); // clears bolt off screen
        
        _score += 1; // adds 1 to the score
        
        pad.tone(800,0.3); // plays a sound when bolt collides
        pad.tone(800,0.3);
    }
}

// Checks the collision between bolt 3 and enemy ship 1
void GameEngine::check_bolt_three_collision_ship_one(FXOS8700CQ &acc,Gamepad &pad)
{
    Vector2D bolt3_pos = _bolt3.get_pos(); // Gets the x and y positions of bolt 3
    Vector2D enemyship_pos1 = _es1.get_pos(); // Gets the x and y positions of enemy ship 1
    Vector2D enemyship_pos3 = _es3.get_pos(); // Gets the x and y positions of enemy ship 3
    
    // Checks if bolt hits enemy ship by checking overlaps
    if (
    (bolt3_pos.x+7 >= enemyship_pos1.x-1) && // right side of bolt
    (bolt3_pos.x <= enemyship_pos1.x+13) && // left side of bolt
    (bolt3_pos.y+1 >= enemyship_pos1.y) && // bottom side of bolt
    (bolt3_pos.y <= enemyship_pos1.y+6) // top side of bolt
    ) {
        // When bolt hits enemy ship, the following actions occur
        Data values = acc.get_values(); // polls the sensor and stores the values in the Data struct        
        srand (values.ay*500); // seeds the rand function with a value from the Data struct multiplied by a large number
        _es1y = rand() % 32 + 3; // generates a random number between 3 and 32 and assigns this value to the y variable 
        _es1.init(enemyship_pos3.x+40,_es1y,_speed);  // Initialises ship 1 40 pixels behind ship 3 in the x direction off screen
        
        _bolt3.clear_bolt(); // clears bolt off screen
        
        _score += 1; // adds 1 to the score
        
        pad.tone(800,0.3); // plays a sound when bolt collides
        pad.tone(800,0.3);
    }
}

// Checks the collision between bolt 1 and enemy ship 2
void GameEngine::check_bolt_one_collision_ship_two(FXOS8700CQ &acc,Gamepad &pad)
{
    Vector2D bolt1_pos = _bolt1.get_pos(); // Gets the x and y positions of bolt 1
    Vector2D enemyship_pos2 = _es2.get_pos(); // Gets the x and y positions of enemy ship 2
    Vector2D enemyship_pos1 = _es1.get_pos(); // Gets the x and y positions of enemy ship 1
    
    // Checks if bolt hits enemy ship by checking overlaps
    if (
    (bolt1_pos.x+7 >= enemyship_pos2.x-1) && // right side of bolt
    (bolt1_pos.x <= enemyship_pos2.x+13) && // left side of bolt
    (bolt1_pos.y+1 >= enemyship_pos2.y) && // bottom side of bolt
    (bolt1_pos.y <= enemyship_pos2.y+6) // top side of bolt
    ) {
        // When bolt hits enemy ship, the following actions occur
        Data values = acc.get_values(); // polls the sensor and stores the values in the Data struct        
        srand (values.ay*500); // seeds the rand function with a value from the Data struct multiplied by a large number
        _es2y = rand() % 32 + 3; // generates a random number between 3 and 32 and assigns this value to the y variable 
        _es2.init(enemyship_pos1.x+40,_es3y,_speed); // Initialises ship 2 40 pixels behind ship 1 in the x direction off screen 
        
        _bolt1.clear_bolt(); // clears bolt off screen
        
        _score += 1; // adds 1 to the score
        
        pad.tone(800,0.3); // plays a sound when bolt collides
        pad.tone(800,0.3);
    }
}

// Checks the collision between bolt 2 and enemy ship 2
void GameEngine::check_bolt_two_collision_ship_two(FXOS8700CQ &acc,Gamepad &pad)
{

    Vector2D bolt2_pos = _bolt2.get_pos(); // Gets the x and y positions of bolt 2
    Vector2D enemyship_pos2 = _es2.get_pos(); // Gets the x and y positions of enemy ship 2
    Vector2D enemyship_pos1 = _es1.get_pos(); // Gets the x and y positions of enemy ship 1
    
    // Checks if bolt hits enemy ship by checking overlaps
    if (
    (bolt2_pos.x+7 >= enemyship_pos2.x-1) && // right side of bolt
    (bolt2_pos.x <= enemyship_pos2.x+13) && // left side of bolt
    (bolt2_pos.y+1 >= enemyship_pos2.y) && // bottom side of bolt
    (bolt2_pos.y <= enemyship_pos2.y+6) // top side of bolt
    ) {
        // When bolt hits enemy ship, the following actions occur
        Data values = acc.get_values(); // polls the sensor and stores the values in the Data struct        
        srand (values.ay*500); // seeds the rand function with a value from the Data struct multiplied by a large number
        _es2y = rand() % 32 + 3; // generates a random number between 3 and 32 and assigns this value to the y variable 
        _es2.init(enemyship_pos1.x+40,_es3y,_speed); // Initialises ship 2 40 pixels behind ship 1 in the x direction off screen
        
        _bolt2.clear_bolt(); // clears bolt off screen
        
        _score += 1; // adds 1 to the score
        
        pad.tone(800,0.3); // plays a sound when bolt collides
        pad.tone(800,0.3);
    }
    
}

// Checks the collision between bolt 3 and enemy ship 2
void GameEngine::check_bolt_three_collision_ship_two(FXOS8700CQ &acc,Gamepad &pad)
{
    Vector2D bolt3_pos = _bolt3.get_pos(); // Gets the x and y positions of bolt 3
    Vector2D enemyship_pos2 = _es2.get_pos(); // Gets the x and y positions of enemy ship 2
    Vector2D enemyship_pos1 = _es1.get_pos(); // Gets the x and y positions of enemy ship 1
    
    // Checks if bolt hits enemy ship by checking overlaps
    if (
    (bolt3_pos.x+7 >= enemyship_pos2.x-1) && // right side of bolt
    (bolt3_pos.x <= enemyship_pos2.x+13) && // left side of bolt
    (bolt3_pos.y+1 >= enemyship_pos2.y) && // bottom side of bolt
    (bolt3_pos.y <= enemyship_pos2.y+6) // top side of bolt
    ) {
        // When bolt hits enemy ship, the following actions occur
        Data values = acc.get_values(); // polls the sensor and stores the values in the Data struct        
        srand (values.ay*500); // seeds the rand function with a value from the Data struct multiplied by a large number
        _es2y = rand() % 32 + 3; // generates a random number between 3 and 32 and assigns this value to the y variable 
        _es2.init(enemyship_pos1.x+40,_es2y,_speed); // Initialises ship 2 40 pixels behind ship 1 in the x direction off screen
        
        _bolt3.clear_bolt(); // clears bolt off screen
        
        _score += 1; // adds 1 to the score
        
        pad.tone(800,0.3); // plays a sound when bolt collides
        pad.tone(800,0.3);
    }
}

// Checks the collision between bolt 1 and enemy ship 3
void GameEngine::check_bolt_one_collision_ship_three(FXOS8700CQ &acc,Gamepad &pad)
{
    Vector2D bolt1_pos = _bolt1.get_pos(); // Gets the x and y positions of bolt 1
    Vector2D enemyship_pos2 = _es2.get_pos(); // Gets the x and y positions of enemy ship 2
    Vector2D enemyship_pos3 = _es3.get_pos(); // Gets the x and y positions of enemy ship 3
    
    // Checks if bolt hits enemy ship by checking overlaps
    if (
    (bolt1_pos.x+7 >= enemyship_pos3.x-1) && // right side of bolt
    (bolt1_pos.x <= enemyship_pos3.x+13) && // left side of bolt
    (bolt1_pos.y+1 >= enemyship_pos3.y) && // bottom side of bolt
    (bolt1_pos.y <= enemyship_pos3.y+6) // top side of bolt
    ) {
        // When bolt hits enemy ship, the following actions occur
        Data values = acc.get_values(); // polls the sensor and stores the values in the Data struct        
        srand (values.ay*500); // seeds the rand function with a value from the Data struct multiplied by a large number
        _es3y = rand() % 32 + 3; // generates a random number between 3 and 32 and assigns this value to the y variable 
        _es3.init(enemyship_pos2.x+40,_es3y,_speed); // Initialises ship 3 40 pixels behind ship 2 in the x direction off screen
        
        _bolt1.clear_bolt(); // clears bolt off screen
        
        _score += 1; // adds 1 to the score
        
        pad.tone(800,0.3); // plays a sound when bolt collides
        pad.tone(800,0.3);
    }
}

// Checks the collision between bolt 2 and enemy ship 3
void GameEngine::check_bolt_two_collision_ship_three(FXOS8700CQ &acc,Gamepad &pad)
{
    Vector2D bolt2_pos = _bolt2.get_pos(); // Gets the x and y positions of bolt 2
    Vector2D enemyship_pos2 = _es2.get_pos(); // Gets the x and y positions of enemy ship 2
    Vector2D enemyship_pos3 = _es3.get_pos(); // Gets the x and y positions of enemy ship 3
    
    // Checks if bolt hits enemy ship by checking overlaps
    if (
    (bolt2_pos.x+7 >= enemyship_pos3.x-1) && // right side of bolt
    (bolt2_pos.x <= enemyship_pos3.x+13) && // left side of bolt
    (bolt2_pos.y+1 >= enemyship_pos3.y) && // bottom side of bolt
    (bolt2_pos.y <= enemyship_pos3.y+6) // top side of bolt
    ) {
        // When bolt hits enemy ship, the following actions occur
        Data values = acc.get_values(); // polls the sensor and stores the values in the Data struct        
        srand (values.ay*500); // seeds the rand function with a value from the Data struct multiplied by a large number
        _es3y = rand() % 32 + 3; // generates a random number between 3 and 32 and assigns this value to the y variable 
        _es3.init(enemyship_pos2.x+40,_es3y,_speed); // Initialises ship 3 40 pixels behind ship 2 in the x direction off screen 
        
        _bolt2.clear_bolt(); // clears bolt off screen
        
        _score += 1; // adds 1 to the score
        
        pad.tone(800,0.3); // plays a sound when bolt collides
        pad.tone(800,0.3);
    }
}

// Checks the collision between bolt 3 and enemy ship 3
void GameEngine::check_bolt_three_collision_ship_three(FXOS8700CQ &acc,Gamepad &pad)
{
    Vector2D bolt3_pos = _bolt3.get_pos(); // Gets the x and y positions of bolt 3
    Vector2D enemyship_pos2 = _es2.get_pos(); // Gets the x and y positions of enemy ship 2
    Vector2D enemyship_pos3 = _es3.get_pos(); // Gets the x and y positions of enemy ship 3
    
    // Checks if bolt hits enemy ship by checking overlaps
    if (
    (bolt3_pos.x+7 >= enemyship_pos3.x-1) && // right side of bolt
    (bolt3_pos.x <= enemyship_pos3.x+13) && // left side of bolt
    (bolt3_pos.y+1 >= enemyship_pos3.y) && // bottom side of bolt
    (bolt3_pos.y <= enemyship_pos3.y+6) // top side of bolt
    ) {
        // When bolt hits enemy ship, the following actions occur
        Data values = acc.get_values(); // polls the sensor and stores the values in the Data struct        
        srand (values.ay*500); // seeds the rand function with a value from the Data struct multiplied by a large number
        _es3y = rand() % 32 + 3; // generates a random number between 3 and 32 and assigns this value to the y variable
        _es3.init(enemyship_pos2.x+40,_es3y,_speed); // Initialises ship 3 40 pixels behind ship 2 in the x direction off screen
        
        _bolt3.clear_bolt(); // clears bolt off screen
        
        _score += 1; // adds 1 to the score
        
        pad.tone(800,0.3); // plays a sound when bolt collides
        pad.tone(800,0.3);
    }
}

// Updates leds 1 to 3 based on number of lives left
void GameEngine::health_bar(Gamepad &pad)
{
    if (_lives == 3){ // When lives are full and set to 3, leds 1 to 3 are switched on
        pad.led(3,1);
        pad.led(2,1);
        pad.led(1,1);
    } if (_lives == 2){ // When lives decreases by 1, led 3 is switched off
        pad.led(3,0);    
    } if (_lives == 1){ // When lives decreases by 1, led 2 is switched off
        pad.led(2,0);
    } if (_lives == 0){ // When lives is equal to 0, led 1 is switched off
        pad.led(1,0);
    }
    
    //printf("lives = %d\n",_lives);
}

// Updates leds 4 to 6 based on how much ammo is left
void GameEngine::ammo_bar(Gamepad &pad)
{
    if (_ammo == 3){
        pad.led(6,1); // When ammo is full, leds 4 to 6 are turned on
        pad.led(5,1);
        pad.led(4,1);
    } if (_ammo == 2){ // When ammo is decreased by 1, led 6 is switched off
        pad.led(6,0);
    } if (_ammo == 1){ // When ammo is decreased by another 1, led 5 is switched off
        pad.led(5,0);
    } if (_ammo == 0){ // When ammo is 0, led 4 is switched off
        pad.led(4,0);
    }   
    
    //printf("lives = %d\n",_lives);
}

// Gets the current score
int GameEngine::get_score()
{
    return _score; // returns the current score
}

// Gets the current number of lives
int GameEngine::get_lives()
{
    return _lives; // returns lives
}

