#include "Spaceship.h"

// constructor and destructor not defined
Spaceship::Spaceship()
{

}

Spaceship::~Spaceship()
{

}

// Initialises the spaceship
void Spaceship::init(int x, int y, int sensitivity)
{
    _x = x; // x position of the spaceship
    _y = y; // y position of the spaceship
    _speed = 1; // initial speed of spaceship
    _sensitivity = sensitivity; // // how fast the bolt updates in the y direction.
}

// Draws the spaceship at _x and _y co-ordinates given
void Spaceship::draw(N5110 &lcd)
{
    static int spaceship_data[] = {
            1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,
            0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,
            0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,
            0,1,1,1,1,1,1,1,1,1,1,1,0,1,1,
            0,0,1,0,0,0,0,0,0,0,0,1,1,1,0,
            0,0,1,0,0,0,0,0,0,0,0,1,1,1,0,
            0,1,1,1,1,1,1,1,1,1,1,1,0,1,1,
            0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,
            0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,
            1,1,1,1,0,0,0,0,0,0,0,0,0,0,0
            };
    Bitmap spaceship(spaceship_data, 10, 15);
    spaceship.render(lcd, _x, _y); //renders the static int Bitmap at _x and _y positions on the lcd screen
        
}

// updates the spaceship in the y direction using the joystick
void Spaceship::update(Direction d,float mag)
{
    _speed = int(mag*_sensitivity); //takes the mag value of the joystick and multiplies by sensitivity to get speed of spaceship in the y direction
    
    // update y value depending on direction of movement
    // North is decrement as origin is at the top-left so decreasing moves up
    if (d == N) {
        _y-=_speed;
    } else if (d == S) {
        _y+=_speed;
    }
    
    // check the y origin to ensure that the paddle doesn't go off screen
    if (_y < 2) {
        _y = 2;
    } 
    if (_y > HEIGHT - 13) {
        _y = HEIGHT - 13;
    }
}

// Gets the position of the spaceship
Vector2D Spaceship::get_pos() {
    Vector2D s = {_x,_y}; //adds the co-ordinates to a Vector2D s and returns s
    return s;    
}
