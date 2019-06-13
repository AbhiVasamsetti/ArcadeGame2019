#ifndef ENEMYSHIPONE_H
#define ENEMYSHIPONE_H

#include "mbed.h"
#include "N5110.h"
#include "Gamepad.h"
#include "Bitmap.h"

/** Enemy ship class
* @brief Controls the enemy ship in the Space Wars game
* @author Abhi Vasamsetti
* @date May 2019
*/

class EnemyShipOne
{
public:

    /** Constructor */
    EnemyShipOne();
    
    /** Destructor */
    ~EnemyShipOne();
    
    /** Initialises the enemy ship
    * @param x co-ordinate (int)
    * @param y co-ordinate (int)
    * @param speed of ship (int)
    */
    void init(int x, int y, int speed);
    
    /** Renders the enemy ship onto the lcd screen
    * @param lcd @details calls the N5110 class using a pointer
    */
    void draw(N5110 &lcd);
    
    /** Updates the x co-ordinates of the enemy ship with the speed */
    void update();
    
    /** Gets x and y co-ordinates of the enemy ship
    * @returns a struct with x and y members
    */
    Vector2D get_pos();
    
    /** Sets the x and y positions of the enemy ship 
    * @param s @details Struct of the x and y positions of the enemy ship
    */
    void set_pos(Vector2D s);
    
private:

    int _x;
    int _y;
    int _speed;
    
};
#endif