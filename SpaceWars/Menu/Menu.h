#ifndef MENU_H
#define MENU_H

#include "mbed.h"
#include "N5110.h"
#include "Gamepad.h"

#include "FXOS8700CQ.h"
#include "GameEngine.h"



/** struct for state (Finite State Machine(FSM) for the menu) */
struct StateMenu {
    int output;  /**< int for output value */
    int nextState[8]; /**< int for array of nextState values */
};

/** Menu class
* @brief Controls the menu options for the Space Wars game
* @author Abhi Vasamsetti
* @date May 2019
*/
class Menu
{

public:  
    /** Constructor */
    Menu();
    
    /** Destructor */
    ~Menu();
    
    /** Initialises all the state, difficulty and sensitivity variable */
    void init();
    
    /** Runs the menu loop with the menu Finite State Machine (FSM)
    * @param pad @details calls the Gamepad class using a pointer 
    * @param lcd @details calls the N5110 class using a pointer
    * @param acc @details calls the accelerometer FXOS8700CQ class using a pointer
    * @param game @details calls the GameEngine class using a pointer
     */
    void run_menu(Gamepad &pad, N5110 &lcd, FXOS8700CQ &acc, GameEngine &game);
    
    /** Reads the button inputs
     * @brief Also reads the number of lives from the GameEngine
     * @param a @details Button A  (bool)
     * @param b @details Button B (bool)
     * @param x @details Button X (bool)
     * @param y @details Button Y (bool)
     * @param back @details Button BACK (bool)
     * @param start @details Button START (bool)
     * @param game @details calls the GameEngine class using a pointer
     * @param pad @details calls the Gamepad class using a pointer 
    */
    void read_input(bool a, bool b, bool x, bool y, bool back, bool start, GameEngine &game, Gamepad &pad);
    
    /** @brief Function that collects all menu functions and calls a specific menu function 
     * @brief based on the output state of the menu FSM. 
     * @param lcd @details calls the N5110 class using a pointer
     * @param acc @details calls the accelerometer FXOS8700CQ class using a pointer
     * @param pad @details calls the Gamepad class using a pointer
     * @param game @details calls the GameEngine class using a pointer
    */
    void run_output(N5110 &lcd,FXOS8700CQ &acc, Gamepad &pad,GameEngine &game);
    
    /** Displays the start up screen on the lcd screen and plays a start up song
     * @param lcd @details calls the N5110 class using a pointer
     * @param pad @details calls the Gamepad class using a pointer
    */
    void splash_screen(N5110 &lcd, Gamepad &pad);
    
    /** Displays the main menu on the lcd screen 
     * @param lcd @details calls the N5110 class using a pointer
    */
    void main_menu(N5110 &lcd);
    
    /** Displays the ready menu on the lcd screen and initialises game engine 
     * @param lcd @details calls the N5110 class using a pointer
     * @param acc @details calls the accelerometer FXOS8700CQ class using a pointer
     * @param pad @details calls the Gamepad class using a pointer
     * @param game @details calls the GameEngine class using a pointer
    */
    void ready_screen(N5110 &lcd,FXOS8700CQ &acc, Gamepad &pad,GameEngine &game);
    
    /** Displays the settings menu on the lcd screen 
     * @param lcd @details calls the N5110 class using a pointer
    */
    void settings_menu(N5110 &lcd);
    
    /** Displays the credits (Name of project creator) on the lcd screen 
     * @param lcd @details calls the N5110 class using a pointer
    */
    void credits(N5110 &lcd);
    
    /** Runs the game engine
     * @param lcd @details calls the N5110 class using a pointer
     * @param acc @details calls the accelerometer FXOS8700CQ class using a pointer
     * @param pad @details calls the Gamepad class using a pointer
     * @param game @details calls the GameEngine class using a pointer
    */
    void run_game(N5110 &lcd,Gamepad &pad,FXOS8700CQ &acc,GameEngine &game);
    
    /** Displays the difficulty menu on the lcd screen 
     * @param lcd @details calls the N5110 class using a pointer
    */
    void difficulty_menu(N5110 &lcd);
    
    /** Displays the brightness menu on the lcd screen
     * @brief Allows use of potentiometer to control backlight of lcd screen
     * @param lcd @details calls the N5110 class using a pointer
     * @param pad @details calls the Gamepad class using a pointer
    */
    void brightness(N5110 &lcd,Gamepad &pad);
    
    /** Displays an easy mode set screen on the lcd screen 
     * @param lcd @details calls the N5110 class using a pointer
    */
    void easy_screen(N5110 &lcd);
    
    /** Displays an medium mode set screen on the lcd screen 
     * @param lcd @details calls the N5110 class using a pointer
    */
    void medium_screen(N5110 &lcd);
    
    /** Displays an hard mode set screen on the lcd screen 
     * @param lcd @details calls the N5110 class using a pointer
    */
    void hard_screen(N5110 &lcd);
    
    /** Displays the pause menu on the lcd screen 
     * @param lcd @details calls the N5110 class using a pointer
    */
    void pause_menu(N5110 &lcd);
    
    /** Displays a game over screen on the lcd screen
     * @brief Also displays the high score from the sd card and displays current score
     * @param lcd @details calls the N5110 class using a pointer
     * @param game @details calls the GameEngine class using a pointer
    */
    void game_over(N5110 &lcd, GameEngine &game);
    
    /** Writes a high score to the sd card
     * @param top_score @details The highest score achieved in the Space Wars game (int)
    */
    void save_score(int top_score);
    
    /** Reads the high score from the sd card
     * @returns Returns the high score (int)
    */
    int read_score();
    
    /** Reads the high score from the sd card and displays it on the lcd screen 
     * @param lcd @details calls the N5110 class using a pointer
     * @param game @details calls the GameEngine class using a pointer
    */
    void score_screen(N5110 &lcd,GameEngine &game);
    
    /** Displays the sensitivity menu on the lcd screen
     * @brief Allows use of potentiometer to control sensitivity of joystick for Space Wars game
     * @param lcd @details calls the N5110 class using a pointer
     * @param pad @details calls the Gamepad class using a pointer
    */
    void sensitivity_screen(N5110 &lcd, Gamepad &pad);
    
    /** Displays the reset score menu on the lcd screen 
     * @param lcd @details calls the N5110 class using a pointer
    */
    void reset_score_screen(N5110 &lcd);
    
    /** Compares the user score and the high score 
     * @param game @details calls the GameEngine class using a pointer
     * @returns true if user score is higher than high score
     * @returns false if high score is higher than user score
    */
    bool compare_score(GameEngine &game);
    
    /** Displays the settings menu with a resume button on the lcd screen 
     * @param lcd @details calls the N5110 class using a pointer
    */
    void pause_settings_menu(N5110 &lcd);


private:

    int _output;
    int _input;
    int _state_menu;

    int _difficulty;
    int _sensitivity;
    
    float pot_read;
    float pot_x;
    
    int _hi_score;

    
    
};
#endif