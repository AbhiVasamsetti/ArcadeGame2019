#include "EnemyShipOne.h"

// constructor and destructor not defined
EnemyShipOne::EnemyShipOne()
{

}

EnemyShipOne::~EnemyShipOne()
{

}

// Initialises the enemy ship with x and y positions and a speed variable
void EnemyShipOne::init(int x, int y, int speed)
{
    
    _x = x; //x co-ordinate of enemy ship
    _y = y; //y co-ordinate of enemy ship
    _speed = speed; //variable that hold speed of enemy ship
}

// Draws the enemy ship at _x and _y co-ordinates given
void EnemyShipOne::draw(N5110 &lcd)
{
    static int enemyshipone_data[] = {
            1,1,0,0,1,1,1,1,1,1,0,0,0,0,
            1,1,0,1,0,0,0,0,0,0,1,0,0,0,
            0,1,1,0,0,0,0,0,0,0,0,1,0,0,
            1,1,1,1,1,1,1,1,1,1,1,1,1,1,
            0,1,1,1,1,1,1,1,1,1,1,1,1,0,
            0,0,0,1,1,1,1,1,1,1,1,0,0,0
            };
        Bitmap enemyshipone(enemyshipone_data, 6, 14); 
        enemyshipone.render(lcd, _x, _y); //renders the static int bitmap at _x and _y positions on the lcd screen
}

// fixes the y position of the enemy ship and updates the x-coordinates of the ship in the negative x-direction
void EnemyShipOne::update()
{
    
    _x-=_speed; // subtracts speed from _x and updates it to the _x variable, thereby moving the ship in the negative x-direction
}

// Gets the position of the enemy ship
Vector2D EnemyShipOne::get_pos() {
    Vector2D s = {_x,_y}; //adds the co-ordinates to a Vector2D s and returns s
    return s;    
}
// Sets the position of the enemy ship at given x and y positions, by updating the elements in the Vector2D struct.
void EnemyShipOne::set_pos(Vector2D s)
{
    _x = s.x; ////updates the y elements of the s struct
    _y = s.y; //updates the y elements of the s struct
}