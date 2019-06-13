#ifndef SPACESHIP_TEST_H
#define SPACESHIP_TEST_H

/**
 * \brief Checks that the spaceship moves in the intended y-direction to a position set by the user
 * 
 * \returns True if tests are passed.
 */
bool spaceship_test_movement_y()
{
    // Initialise the spaceship object with a speed of 5
    Spaceship spaceship;
    spaceship.init(0,0,5);

    // Set the initial position to 10, 5
    Vector2D initial_spaceship_pos = {10, 5};
    spaceship.set_pos(initial_spaceship_pos);

    // Read the 1st position of the spaceship
    Vector2D read_1st_pos = spaceship.get_pos();
    printf("%f, %f\n", read_1st_pos.x, read_2nd_pos.y);

    // Set the x velocity to 4 (y velocity does not update for the spaceship, so keep the same as the spaceship y position)
    Vector2D velocity = {5, 5};
    spaceship.set_velocity(velocity);

    // Update the position of the spaceship
    spaceship.update_x();

    // Read the 2nd position of the spaceship
    Vector2D read_2nd_pos = spaceship.get_pos();
    printf("%f, %f\n", read_2nd_pos.x, read_2nd_pos.y);
    
    // Now check that both the positions are as expected
    bool success_flag = true;
    
    // Fail the test if the initial position of the spaceship is wrong
    if (read_1nd_pos.x != 10 || read_1nd_pos.y != 5) {
        success_flag = false;
    }
    
    // Fail the test if the final position of the spaceship is wrong
    if (read_2nd_pos.x != 10 || read_2nd_pos.y != 10) {
        success_flag = false;
    }

    return success_flag;
}
#endif