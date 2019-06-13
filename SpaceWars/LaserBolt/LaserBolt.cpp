#include "LaserBolt.h"

// constructor and destructor not defined
LaserBolt::LaserBolt()
{

}

LaserBolt::~LaserBolt()
{

}

// Initialises the bolt
void LaserBolt::init(int x, int y, int width, int speed, int sensitivity)
{
    _x = x; // x position of bolt
    _y = y; // y position of bolt
    _width = width; // width of bolt
    _size = 2; // height of bolt
    _sensitivity = sensitivity; // how fast the bolt updates in the y direction
    _velocity.x = speed; // initialises the x velocity of the bolt with the speed 
}

// Renders the bolt at the x and y positions of the bolt on the lcd screen
void LaserBolt::draw(N5110 &lcd)
{
    lcd.drawRect(_x,_y,_width,_size,FILL_BLACK);
}

// clears the bolt off screen
void LaserBolt::clear_bolt()
{
    Vector2D bolt_pos = get_pos(); //gets the current position of the bolt
    bolt_pos.y = 300; // changes the y position of the bolt to an extremely large value (off screen)
    set_pos(bolt_pos); //updates the bolt positions with the new y value
}

// updates the bolt in the y direction using the joystick
void LaserBolt::initial_update_y(Direction d,float mag)
{
    _speed = int(mag*_sensitivity); //takes the mag value of the joystick and multiplies by sensitivity to get speed of bolt in the y direction
    
    // update y value depending on direction of movement
    // North is decrement as origin is at the top-left so decreasing moves up
    if (d == N) {
        _y-=_speed;
    } else if (d == S) {
        _y+=_speed;
    }
    
    // check the y origin to ensure that the paddle doesn't go off screen
    if (_y < 6) {
        _y = 6;
    }
    if (_y > HEIGHT - 9) {
        _y = HEIGHT - 9;
    }

}

// fixes the y direction of the bolt and updates in the x direction 
void LaserBolt::update_x()
{
    _x += _velocity.x; // updates the x direction by adding the x velocity to the x position
}

// Sets the velocity of the bolt at given x and y velocities, by updating the elements in the Vector2D struct.
void LaserBolt::set_velocity(Vector2D v)
{
    _velocity.x = v.x; //updates the x elements of the p struct
    _velocity.y = v.y; //updates the y elements of the p struct
}

// Gets the velocities of the bolt
Vector2D LaserBolt::get_velocity()
{
    Vector2D v = {_velocity.x,_velocity.y}; //adds the x and y velocities to a Vector2D struct v and returns v
    return v;
}

// Gets the position of the bolt
Vector2D LaserBolt::get_pos()
{
    Vector2D p = {_x,_y}; //adds the co-ordinates to a Vector2D struct p and returns p
    return p;
}

// Sets the position of the bolt at given x and y positions, by updating the elements in the Vector2D struct.
void LaserBolt::set_pos(Vector2D p)
{
    _x = p.x; //updates the x elements of the p struct
    _y = p.y; //updates the y elements of the p struct
}
