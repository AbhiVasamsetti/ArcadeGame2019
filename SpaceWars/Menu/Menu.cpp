#include "Menu.h"
#include "SDFileSystem.h"

/////////////// objects ///////////////
SDFileSystem sd(PTE3, PTE1, PTE2, PTE4, "sd"); // MOSI, MISO, SCK, CS
Serial serial(USBTX, USBRX);

// SPACE WARS sprite
int splash_sprite[7][54] = {
    {1,1,1,1,0,1,1,1,1,1,0,1,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1,0,0,0,1,0,0,0,0,1,0,0,1,1,0,0,1,1,1,1,1,0,1,1,1,1,0,0},
    {1,0,0,0,0,1,0,0,0,1,0,1,0,0,1,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,1,0,1,0,0,1,0,1,0,0,0,1,0,1,0,0,0,0,0},
    {1,0,0,0,0,1,0,0,0,1,0,1,0,0,1,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,1,0,1,0,0,1,0,1,0,0,0,1,0,1,0,0,0,0,0},
    {1,1,1,1,0,1,1,1,1,1,0,1,1,1,1,0,1,0,0,0,0,0,1,1,1,1,1,0,0,0,1,0,0,0,0,1,0,1,1,1,1,0,1,1,1,1,1,0,1,1,1,1,0,0},
    {0,0,0,1,0,1,0,0,0,0,0,1,0,0,1,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,1,0,1,0,0,1,0,1,0,0,1,0,0,0,0,0,1,0,0},
    {0,0,0,1,0,1,0,0,0,0,0,1,0,0,1,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,1,1,0,1,0,1,0,0,1,0,1,0,0,0,1,0,0,0,0,1,0,0},
    {1,1,1,1,0,1,0,0,0,0,0,1,0,0,1,0,1,1,1,1,1,0,1,1,1,1,1,0,0,0,1,1,0,0,1,1,0,1,0,0,1,0,1,0,0,0,1,0,1,1,1,1,0,0}
    };
        

// Finite State Machine (FSM/0 for menu
StateMenu _fsm_menu[17] = {
    {0, {   0,  0,  0,  0,  0,  1,  0, 0}},
    {1, {   2,  3,  4,  13,  0,  1,  1, 1}},
    {2, {   2,  2,  2,  2,  1,  5,  2, 2}},
    {3, {   6,  7,  14,  3,  1,  3,  3, 3}},
    {4, {   4,  4,  4,  4,  0,  4,  4, 4}},
    {5, {   5,  5,  5,  5,  11, 5,  5, 12}},
    {6, {   8,  9,  10, 6,  3,  6,  6, 6}},
    {7, {   3,  7,  7,  7,  3,  7,  7, 7}},
    {8, {   8,  8,  8,  8,  6,  2,  8, 8}},
    {9, {   9,  9,  9,  9,  6,  2,  9, 9}},
    {10, {  10, 10, 10, 10, 6,  2,  10,10}},
    {11, {  4,  16,  11, 11, 2,  5,  11, 11}},
    {12, {  12, 12, 12, 12, 2,  12, 12, 12}},
    {13, { 15, 13, 13, 13, 1, 13, 13, 13}},
    {14, { 3, 14, 14, 14, 3, 14, 14, 14}},
    {15, {13, 13, 13, 13, 13, 13, 15, 15}},
    {16, {6, 7, 14, 5, 11, 16, 16, 16}}
    };

// constructor and destructor not defined
Menu::Menu()
{
    
}

Menu::~Menu()
{

}

// Initialises menu
void Menu::init()   
{
    _state_menu = 0; // Sets initial output state to 0
    _difficulty = 2; // Sets initial difficulty to 2
    _sensitivity = 4; // Sets initial sensitivity to 4
}

// Runs the menu in a loop
void Menu::run_menu(Gamepad &pad, N5110 &lcd, FXOS8700CQ &acc,GameEngine &game)
{
    read_input(pad.check_event(Gamepad::A_PRESSED), 
               pad.check_event(Gamepad::B_PRESSED), 
               pad.check_event(Gamepad::X_PRESSED), 
               pad.check_event(Gamepad::Y_PRESSED), 
               pad.check_event(Gamepad::BACK_PRESSED), 
               pad.check_event(Gamepad::START_PRESSED),
               game,pad);
               
    // Sets the output state of the menu FSM based on the current state
    _output = _fsm_menu[_state_menu].output;
    
    // Runs the output functions of the menu FSM based on the output state
    run_output(lcd,acc,pad,game);
    
    // Sets the next state of the menu FSM based on the input and current state
    _state_menu = _fsm_menu[_state_menu].nextState[_input]; 
    
    //printf("output state = %d\n",_output);
    //printf("input state = %d\n",_input);
}

// Reads the inputs from the pad, collects the lives from the game and changes the input of the menu
void Menu::read_input(bool a, bool b, bool x, bool y, bool back, bool start, GameEngine &game, Gamepad &pad)
{
    int lives = game.get_lives(); // gets the lives from the game engine
    //printf("lives = %d\n",lives);
    
    // When one of the buttons is pressed, there is a sound
    if (a && !b && !x && !y && !back && !start){ // button A pressed 
        _input = 0; 
        pad.tone(1200,0.1);
    } else if (!a && b && !x && !y && !back && !start) { // button B pressed 
        _input = 1;
        pad.tone(1200,0.1);
    } else if (!a && !b && x && !y && !back && !start) { // button X pressed 
        _input = 2;
        pad.tone(1200,0.1);
    } else if (!a && !b && !x && y && !back && !start) { // button Y pressed 
        _input = 3;
        pad.tone(1200,0.1);
    } else if (!a && !b && !x && !y && back && !start) { // button BACK pressed 
        _input = 4;
        pad.tone(600,0.1);
    } else if (!a && !b && !x && !y && !back && start) { // button START pressed 
        _input = 5;
        pad.tone(1200,0.1);
    } else if (!a && !b && !x && !y && !back && !start && (lives == 0)) { // when lives = 0
        _input = 7;
    } else if (!a && !b && !x && !y && !back && !start && !(lives == 0)) { // no button pressed
        _input = 6;  
    }
}

// Takes the output state of the menu FSM and outputs corresponding functions/actions
void Menu::run_output(N5110 &lcd,FXOS8700CQ &acc, Gamepad &pad,GameEngine &game)
{
    if (_output == 0){
        splash_screen(lcd,pad);
    } else if (_output == 1){
        main_menu(lcd);
    } else if (_output == 2){
        ready_screen(lcd,acc,pad,game);
    } else if (_output == 3){
        settings_menu(lcd);
    } else if (_output == 4){
        credits(lcd);
    } else if (_output == 5){
        run_game(lcd,pad,acc,game);
    } else if (_output == 6){
        difficulty_menu(lcd);
    } else if (_output == 7){
        brightness(lcd,pad);
    } else if (_output == 8){
        easy_screen(lcd);
    } else if (_output == 9){
        medium_screen(lcd);
    } else if (_output == 10){
        hard_screen(lcd);
    } else if (_output == 11){
        pause_menu(lcd);
    } else if (_output == 12){
        game_over(lcd,game);
        // Flashes leds to show game over
        pad.leds_on();
        wait(0.1);
        pad.leds_off();
        wait(0.1);
    } else if (_output == 13){
        score_screen(lcd,game);
    } else if (_output == 14) {
        sensitivity_screen(lcd,pad);
    } else if (_output == 15) {
        reset_score_screen(lcd);
    } else if (_output == 16) {
        pause_settings_menu(lcd);
    }
}

// Initial splash screen when game is started up
void Menu::splash_screen(N5110 &lcd, Gamepad &pad)
{
    // Draws the SPACE WARS sprite onto the screen and prints button message
    lcd.drawSprite(16,9,7,54,(int*)splash_sprite);
    lcd.printString("Press START",9,4);
    
    // Start up jingle
    pad.tone(300,2);
    wait(0.5);
    pad.tone(300,2);
    wait(0.5);
    pad.tone(100,2);
    wait(0.5);
}

// Main menu screen with options
void Menu::main_menu(N5110 &lcd)
{
    // Prints all options
    lcd.printString("A: Start game",3,1);
    lcd.printString("B: Settings",8,2); 
    lcd.printString("X: Credits",10,3);
    lcd.printString("Y: High Score",3,4);
}

// A ready screen with controls on how to play
void Menu::ready_screen(N5110 &lcd, FXOS8700CQ &acc, Gamepad &pad, GameEngine &game)
{
    // Prints controls
    lcd.printString("L to fire",13,1);
    lcd.printString("R to reload",9,2);
    lcd.printString("Press START",8,4);
    
    // Initialises game engine
    game.init(_difficulty,lcd,acc,pad,_sensitivity); 
}

// A settings screen without a resume button
void Menu::settings_menu(N5110 &lcd)
{
    // Prints all options
    lcd.printString("A: Difficulty",2,1);
    lcd.printString("B: Brightness",2,2);
    lcd.printString("X: Sensitivity",1,3);
}

// Credits screen to show project creator
void Menu::credits(N5110 &lcd)
{
    lcd.printString("Made by Abhi",3,1);
    lcd.printString("Vasamsetti",3,2);
    lcd.printString("BACK: Return",2,4);
}

// Runs the game engine
void Menu::run_game(N5110 &lcd,Gamepad &pad,FXOS8700CQ &acc,GameEngine &game)
{
    lcd.clear();
    game.run_engine(pad,lcd,acc);
    lcd.refresh();
}

// Difficulty menu screen
void Menu::difficulty_menu(N5110 &lcd)
{
    // Prints all difficulty options
    lcd.printString("A: EASY",20,1);
    lcd.printString("B: MEDIUM",14,2);
    lcd.printString("X: HARD",20,3);
}

// Brightness menu screen that shows option to change brightness of backlight
void Menu::brightness(N5110 &lcd,Gamepad &pad)
{
    // Prints instructions
    lcd.printString("Turn knob to",5,2);
    lcd.printString("set brightness",1,3);
    
    // Reads potentiometer and updates backlight brightness of screen with reading
    pot_read = pad.read_pot();
    lcd.setBrightness(pot_read);
    
}

// A screen to show easy difficulty has been set
void Menu::easy_screen(N5110 &lcd)
{
    // Sets the difficulty to 2
    _difficulty = 2;
    
    // Prints instructions
    lcd.printString("EASY SET",18,1);
    lcd.printString("Press START",10,3);
    lcd.printString("to begin",14,4);
}

// A screen to show medium difficulty has been set
void Menu::medium_screen(N5110 &lcd)
{
    // Sets the difficulty to 3
    _difficulty = 3;
    
    // Prints instructions
    lcd.printString("MEDIUM SET",12,1);
    lcd.printString("Press START",10,3);
    lcd.printString("to begin",14,4);
}

// A screen to show hard difficulty has been set
void Menu::hard_screen(N5110 &lcd)
{
    // Sets the difficulty to 4
    _difficulty = 4;
    
    // Prints instructions
    lcd.printString("HARD SET",18,1);
    lcd.printString("Press START",10,3);
    lcd.printString("to begin",14,4);
}

// A pause menu screen 
void Menu::pause_menu(N5110 &lcd)
{
    // Prints all options
    lcd.printString("START: Resume",3,1);
    lcd.printString("BACK: Restart",3,2);
    lcd.printString("B: Settings",9,3);
    lcd.printString("A: Quit",16,4);
}

// A settings menu screen with a resume button
void Menu::pause_settings_menu(N5110 &lcd)
{ 
    // Prints all options
    lcd.printString("A: Difficulty",2,1);
    lcd.printString("B: Brightness",2,2);
    lcd.printString("X: Sensitivity",1,3);
    lcd.printString("Y: Resume",13,4);
}

// A game over screen that shows user score and high score
void Menu::game_over(N5110 &lcd, GameEngine &game)
{
    // Gets the score from the game
    int score = game.get_score();
    
    if (compare_score(game)) { 
        _hi_score = score; // user score is equal to the high score
        save_score(_hi_score); // saves the high score to the sd card
    } else {
        _hi_score = read_score(); // reads the sd card score to the high score
    }
    
    // Prints the user and high score
    char buffer1[14];
    char buffer2[14];
    sprintf(buffer1,"%2d",score);
    sprintf(buffer2,"%2d",_hi_score);
    lcd.printString(buffer1,6,3);
    lcd.printString(buffer2,54,3);
    

    lcd.printString("GAME OVER",14,1); // prints game over message
    lcd.printString("User Hi-Score",3,2); // prints headings for user and high scores
    lcd.printString("BACK: Restart",3,4); // prints option to restart
    
    // printf("User Score = %d\n",_hi_score);
        
}

// Saves the high score to the sd card
void Menu::save_score(int top_score)
{
    serial.baud(115200);
    FILE *fp;
    
    fp = fopen("/sd/topscore.txt", "w");

    if (fp == NULL) {  // if it can't open the file then print error message
        serial.printf("Error! Unable to open file!\n");
    } else {  // opened file so can write
        serial.printf("Writing to file....");
        fprintf(fp, "%d",top_score); // ensure data type matches
        serial.printf("Done.\n");
        fclose(fp);  // ensure you close the file after writing
    }
}

// Reads the sd card score and returns this score
int Menu::read_score()
{
    int sd_score;
    serial.baud(115200);
    FILE *fp;
    // now open file for reading
    fp = fopen("/sd/topscore.txt", "r");

    if (fp == NULL) {  // if it can't open the file then print error message
        serial.printf("Error! Unable to open file!\n");
    } else {  // opened file so can write
        fscanf(fp, "%d",&sd_score); // ensure data type matches - note address operator (&)
        serial.printf("Read %d from file.\n",sd_score);
        fclose(fp);  // ensure you close the file after reading
    }
    return sd_score;
}

// High score screen to show the high score
void Menu::score_screen(N5110 &lcd,GameEngine &game)
{
    // Reads the high score from the sd card
    int score = read_score();
    //printf("sd score = %d\n",score);
    
    // Prints the high score to the screen
    char buffer3[14];
    sprintf(buffer3,"%2d",score);
    lcd.printString(buffer3,60,1);
    
    // Prints message to show high score and option to reset score
    lcd.printString("Hi-Score:",2,1);
    lcd.printString("Press A to",10,3);
    lcd.printString("reset score",8,4);
    
}

// Reset score screen to show high score has been reset
void Menu::reset_score_screen(N5110 &lcd)
{
    save_score(0); // saves a score of 0 to the sd card
    
    
    // Prints a message to show reset is confirmed
    lcd.printString("Hi-Score",16,1);
    lcd.printString("reset",25,2);
    lcd.printString("confirmed",13,3);
    
}

// Sensitivity screen to set sensitivity of joystick
void Menu::sensitivity_screen(N5110 &lcd, Gamepad &pad)
{
     
     // Prints instructions to turn potentiometer to set sensitivity
     lcd.printString("Turn knob for",5,1);
     lcd.printString("sensitivity",8,2);
     lcd.printString("of joystick",7,3);
     
     // Reads the potentiometer and sets sensitivity of joystick with reading
     pot_read = pad.read_pot();
     _sensitivity = pot_read * 8;
     lcd.drawRect(7,34,72*pot_read,7,FILL_BLACK); // Draws a rectangle with the width corresponding to the potentiometer reading
     lcd.drawRect(7,34,72,7,FILL_TRANSPARENT);
     
}

// Compares the user score and the sd card high score
bool Menu::compare_score(GameEngine &game)
{
    int score = game.get_score(); // gets the current user score from the game
    int sd_score = read_score(); // reads the high score from the sd card
    
    if (score >= sd_score) { // if the user score is bigger than or equal to the sd card score
        return true;
    } else { 
        return false;
    }
}