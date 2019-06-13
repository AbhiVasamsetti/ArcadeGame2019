#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include "mbed.h"
#include "N5110.h"
#include "Gamepad.h"
#include "Spaceship.h"
#include "EnemyShipOne.h"
#include "FXOS8700CQ.h"
#include "LaserBolt.h"



// gap from edge of screen
#define GAP 0

/** struct for state (Finite State Machine(FSM) for bolts) */
struct StateBolt {
    int output;  /**< int for output value */
    int nextState[3];  /**< int for array of nextState values */
};

/** struct for state (FSM for enemy ships) */
struct StateEnemy {
    int output;  /**< int for output value */
    int nextState[3];  /**< int for array of nextState values */
};

/** Game engine class
* @brief The main game engine for the Space Wars game
* @author Abhi Vasamsetti
* @date May 2019
*/

class GameEngine
{

public:
    /** Constructor */
    GameEngine();
    
    /** Destructor */
    ~GameEngine();
    
    /** Initialises the game engine, all objects and variables
    * @param difficulty (int) @details The difficulty is the speed of all the ships
    * @param lcd @details calls the N5110 class using a pointer
    * @param acc @details calls the accelerometer FXOS8700CQ class using a pointer
    * @param sensitivity (int) @details controls the sensitivity of the user spaceship by updating the speed of the spaceship
    */
    void init(int difficulty, N5110 &lcd, FXOS8700CQ &acc, Gamepad &pad, int sensitivity);
    
    /** Runs the main game engine loop
    * @param pad @details calls the Gamepad class using a pointer
    * @param lcd @details calls the N5110 class using a pointer
    * @param acc @details calls the accelerometer FXOS8700CQ class using a pointer
    */
    void run_engine(Gamepad &pad, N5110 &lcd, FXOS8700CQ &acc);
    
    /** Reads the inputs of the game engine
    * @brief Reads the joystick, the R and L button inputs, and the input functions of the bolt and enemy ships
    * @param pad @details calls the Gamepad class using a pointer
    */
    void read_input(Gamepad &pad);
    
    /** Updates the output states of the finite state machines (FSM) of both the bolt and the enemyships */
    void output_object();
    
    /** Updates the output functions using the FSM for the bolt and enemy ships
    * @param acc @details calls the accelerometer FXOS8700CQ class using a pointer
    */
    void run_output(FXOS8700CQ &acc);
    
    /** Renders all the objects onto the lcd screen
    * @param lcd @details calls the N5110 class using a pointer
    */ 
    void draw(N5110 &lcd);
    
    /** Outputs the input states of the enemy ship
    * @param pad @details calls the Gamepad class using a pointer
    */
    void input_ships(Gamepad &pad);
    
    /** Outputs the input states of the bolt
    * @param l @details Button L (bool)
    * @param r @details Button R (bool)
    * @param pad @details calls the Gamepad class using a pointer
    */
    void input_bolts(bool l, bool r,Gamepad &pad);
    
    /** Updates the states of the finite state machines of both the bolt and the enemyships from the input states of both */
    void state_object();
    
    /** Collects all the collision functions 
    * @param pad @details calls the Gamepad class using a pointer
    * @param acc @details calls the accelerometer FXOS8700CQ class using a pointer
    */
    void check_all_collisions(Gamepad &pad, FXOS8700CQ &acc);
    
    /** Collects all the spaceship collision functions
    * @param pad @details calls the Gamepad class using a pointer
    * @param acc @details calls the accelerometer FXOS8700CQ class using a pointer
    */
    void check_spaceship_collisions(Gamepad &pad, FXOS8700CQ &acc);
    
    /** Collects all the bolt collision functions with all the ships
    * @param acc @details calls the accelerometer FXOS8700CQ class using a pointer
    * @param pad @details calls the Gamepad class using a pointer
    */
    void check_bolt_collisions(FXOS8700CQ &acc,Gamepad &pad);
    
    /** Collects all the bolt collision functions with enemy ship one
    * @param acc @details calls the accelerometer FXOS8700CQ class using a pointer
    * @param pad @details calls the Gamepad class using a pointer
    */
    void check_bolt_collision_ship_one(FXOS8700CQ &acc,Gamepad &pad);
    
    /** Collects all the bolt collision functions with enemy ship two
    * @param acc @details calls the accelerometer FXOS8700CQ class using a pointer
    * @param pad @details calls the Gamepad class using a pointer
    */
    void check_bolt_collision_ship_two(FXOS8700CQ &acc,Gamepad &pad);
    
    /** Collects all the bolt collision functions with enemy ship three
    * @param acc @details calls the accelerometer FXOS8700CQ class using a pointer
    * @param pad @details calls the Gamepad class using a pointer
    */
    void check_bolt_collision_ship_three(FXOS8700CQ &acc,Gamepad &pad);
    
    /** Function that outputs various actions when an output state of the enemy ships FSM is acheived*/
    void output_ships(FXOS8700CQ &acc);
    
    /** Function that outputs various actions when an output state of the bolts FSM is acheived*/
    void output_bolts();
    
    /** Checks collisions with enemy ship one
    * @param pad @details calls the Gamepad class using a pointer
    * @param acc @details calls the accelerometer FXOS8700CQ class using a pointer
    */
    void check_spaceship_collisions_one(Gamepad &pad, FXOS8700CQ &acc);
    
    /** Checks collisions with enemy ship two 
    * @param pad @details calls the Gamepad class using a pointer
    * @param acc @details calls the accelerometer FXOS8700CQ class using a pointer
    */
    void check_spaceship_collisions_two(Gamepad &pad, FXOS8700CQ &acc);
    
    /** Checks collisions with enemy ship three
    * @param pad @details calls the Gamepad class using a pointer
    * @param acc @details calls the accelerometer FXOS8700CQ class using a pointer
    */
    void check_spaceship_collisions_three(Gamepad &pad, FXOS8700CQ &acc);
    
    /** Checks for collision between bolt one and ship one
    * @param acc @details calls the accelerometer FXOS8700CQ class using a pointer
    * @param pad @details calls the Gamepad class using a pointer
    */
    void check_bolt_one_collision_ship_one(FXOS8700CQ &acc,Gamepad &pad);
    
    /** Checks for collision between bolt two and ship one
    * @param acc @details calls the accelerometer FXOS8700CQ class using a pointer
    * @param pad @details calls the Gamepad class using a pointer
    */
    void check_bolt_two_collision_ship_one(FXOS8700CQ &acc,Gamepad &pad);
    
    /** Checks for collision between bolt three and ship one
    * @param acc @details calls the accelerometer FXOS8700CQ class using a pointer
    * @param pad @details calls the Gamepad class using a pointer
    */
    void check_bolt_three_collision_ship_one(FXOS8700CQ &acc,Gamepad &pad);
    
    /** Checks for collision between bolt one and ship two
    * @param acc @details calls the accelerometer FXOS8700CQ class using a pointer
    * @param pad @details calls the Gamepad class using a pointer
    */
    void check_bolt_one_collision_ship_two(FXOS8700CQ &acc,Gamepad &pad);
    
    /** Checks for collision between bolt two and ship two
    * @param acc @details calls the accelerometer FXOS8700CQ class using a pointer
    * @param pad @details calls the Gamepad class using a pointer
    */
    void check_bolt_two_collision_ship_two(FXOS8700CQ &acc,Gamepad &pad);
    
    /** Checks for collision between bolt three and ship two
    * @param acc @details calls the accelerometer FXOS8700CQ class using a pointer
    * @param pad @details calls the Gamepad class using a pointer
    */
    void check_bolt_three_collision_ship_two(FXOS8700CQ &acc,Gamepad &pad);
    
    /** Checks for collision between bolt one and ship three
    * @param acc @details calls the accelerometer FXOS8700CQ class using a pointer
    * @param pad @details calls the Gamepad class using a pointer
    */
    void check_bolt_one_collision_ship_three(FXOS8700CQ &acc,Gamepad &pad);
    
    /** Checks for collision between bolt two and ship three
    * @param acc @details calls the accelerometer FXOS8700CQ class using a pointer
    * @param pad @details calls the Gamepad class using a pointer
    */
    void check_bolt_two_collision_ship_three(FXOS8700CQ &acc,Gamepad &pad);
    
    /** Checks for collision between bolt three and ship three
    * @param acc @details calls the accelerometer FXOS8700CQ class using a pointer
    * @param pad @details calls the Gamepad class using a pointer
    */
    void check_bolt_three_collision_ship_three(FXOS8700CQ &acc,Gamepad &pad);
    
    /** Updates the leds of the Gamepad based on the number of lives 
    * @param pad @details calls the Gamepad class using a pointer
    */
    void health_bar(Gamepad &pad);
    
    /** Updates the leds of the Gamepad based on the amount of ammo 
    * @param pad @details calls the Gamepad class using a pointer
    */
    void ammo_bar(Gamepad &pad);

    /** Gets the score 
    * @returns current score
    */
    int get_score();
    
    /** Gets the lives
    * @returns current lives
    */
    int get_lives();
    
private:
    Spaceship _s1;
    EnemyShipOne _es1;
    EnemyShipOne _es2;
    EnemyShipOne _es3;
    LaserBolt _bolt1;
    LaserBolt _bolt2;
    LaserBolt _bolt3;
    
    int _s1x;
    int _s1y;
    int _es1x;
    int _es1y;
    int _es2x;
    int _es2y;
    int _es3x;
    int _es3y;
    
    Direction _d;
    float _mag;
    int _speed;
    int _sensitivity;
    int _score;
    int _lives;
    int _ammo;
    int end_score;
    int current_lives;
    int collision;

    bool _a_pressed;
    bool _start_pressed;
    bool _r_pressed;
    
    int state_bolt;
    int input_bolt;
    int output_bolt;
    int state_enemy;
    int input_enemy;
    int output_enemy;
};
#endif