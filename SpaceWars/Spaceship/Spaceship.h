#ifndef SPACESHIP_H
#define SPACESHIP_H

#include "mbed.h"
#include "N5110.h"
#include "Gamepad.h"
#include "Bitmap.h"

/** Enemy ship class
* @brief Controls the spaceship in the Space Wars game
* @author Abhi Vasamsetti
* @date May 2019
*/
class Spaceship
{
public:
    /** Constructor */
    Spaceship();
    
    /** Destructor */
    ~Spaceship();
    
    /** Initialises the spaceship
    * @param x co-ordinate (int)
    * @param y co-ordinate (int)
    * @param sensitivity (int) @details controls the sensitivity of the user spaceship by updating the speed of the spaceship
    */
    void init(int x,int y, int sensitivty);
    
    /** Renders the spaceship onto the lcd screen
    * @param lcd @details calls the N5110 class using a pointer
    */
    void draw(N5110 &lcd);
    
    /** Updates the y co-ordinates of the spaceship with the sensitivity 
    * @param d @details Direction of the joystick using the Direction enum
    * @param mag @details Magnitude of the joystick movement (float)
    */
    void update(Direction d,float mag);
    
    /** Gets x and y co-ordinates of the spaceship
    * @returns a struct with x and y members
    */
    Vector2D get_pos();

    
private:

    int _x;
    int _y;
    int _speed;
    int _sensitivity;
    
};
#endif