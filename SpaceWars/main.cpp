/* 
ELEC2645 Embedded Systems Project 
School of Electronic & Electrical Engineering 
University of Leeds 
 
Name: Abhi Vasamsetti
Username: el17a2v
Student ID Number: 201198847
Date: 25/03/19
 */
#include "mbed.h"
#include "Gamepad.h"
#include "N5110.h"
#include "Menu.h"

#ifdef WITH_TESTING
# include "tests.h"
#endif


/////////////// structs //////////////////
struct UserInput {
    Direction d;
    float mag;
};

/////////////// objects ///////////////
N5110 lcd(PTC9,PTC0,PTC7,PTD2,PTD1,PTC11);
Gamepad pad; 
FXOS8700CQ acc(I2C_SDA,I2C_SCL);
Menu menu;
GameEngine game;

////////////// prototypes /////////////
void init();

int main()
{
    #ifdef WITH_TESTING
    int number_of_failures = run_all_tests();

    if(number_of_failures > 0) return number_of_failures;
    #endif
    
    init();
    while (1) {
        // Loop
        lcd.clear();
        menu.run_menu(pad,lcd,acc,game); // run the menu in a loop
        lcd.refresh();
        wait(1.0f/15);
        
    }
    
}

// Initialise all peripherals and the menu
void init()
{
    lcd.init();
    lcd.inverseMode();
    lcd.setContrast(0.5);
    pad.init();
    menu.init();
    
}

