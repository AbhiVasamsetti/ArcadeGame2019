#ifndef LASERBOLT_H
#define LASERBOLT_H

#include "mbed.h"
#include "N5110.h"
#include "Gamepad.h"
#include "Spaceship.h"

/** Laser bolt class
* @brief Controls the laser bolt in the Space Wars game
* @author Abhi Vasamsetti
* @date May 2019
*/

class LaserBolt
{

public:
    /** Constructor */
    LaserBolt();
    
    /** Destructor */
    ~LaserBolt();
    
    /** Initialises the enemy ship
    * @param x co-ordinate (int)
    * @param y co-ordinate (int)
    * @param width of bolt (int)
    * @param speed of bolt (int)
    * @param sensitivity (int) @details controls the sensitivity of the bolt 
    */
    void init(int x, int y, int width, int speed, int sensitivity);
    
    /** Renders the bolt onto the lcd screen
    * @param lcd @details calls the N5110 class using a pointer
    */
    void draw(N5110 &lcd);
    
    /** Updates the y co-ordinates of the bolt with the sensitivity 
    * @param d @details Direction of the joystick
    * @param mag (float) @details Magnitude of the joystick movement
    */
    void initial_update_y(Direction d,float mag);
    
    /** Updates the x co-ordinates of the bolt with the x velocity */
    void update_x();

    /** Sets the x and y velocities of the bolt 
    * @param v @ details Struct of the x and y velocities of the bolt
    */
    void set_velocity(Vector2D v);
    
    /** Gets x and y velocities of the bolt
    * @returns a struct with x and y speeds of the bolt
    */
    Vector2D get_velocity();
    
    /** Gets x and y co-ordinates of the bolt
    * @returns a struct with x and y positions of the bolt
    */
    Vector2D get_pos();
    
    /** Sets the x and y positions of the bolt 
    * @param p @ details Struct of the x and y positions of the bolt
    */
    void set_pos(Vector2D p);
    
    /** Gets the y position of the bolt and clears it off screen by adding an extremely large y value*/
    void clear_bolt();
    
private:

    Spaceship _s1;
    Vector2D _velocity;
    
    int _size;
    int _width;
    int _x;
    int _y;
    int _speed;
    int _sensitivity;

};
#endif