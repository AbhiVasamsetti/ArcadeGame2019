#ifndef LASER_BOLT_TEST_H
#define LASER_BOLT_TEST_H

/**
 * \brief Checks that the laser bolt moves in the intended x-direction to a position set by the user
 * 
 * \returns True if tests are passed.
 */
bool bolt_test_movement_x()
{
    // Initialise the enemy object with a width of 6, height of 2, and a speed of 5
    LaserBolt bolt;
    bolt.init(0,0,6,2,5);

    // Set the initial position to 10, 5
    Vector2D initial_bolt_pos = {10, 5};
    bolt.set_pos(initial_bolt_pos);

    // Read the 1st position of the bolt
    Vector2D read_1st_pos = bolt.get_pos();
    printf("%f, %f\n", read_1st_pos.x, read_2nd_pos.y);

    // Set the x velocity to 4 (y velocity does not update for the bolt, so keep the same as the bolt y position)
    Vector2D velocity = {5, 5};
    bolt.set_velocity(velocity);

    // Update the position of the bolt
    bolt.update_x();

    // Read the 2nd position of the bolt
    Vector2D read_2nd_pos = bolt.get_pos();
    printf("%f, %f\n", read_2nd_pos.x, read_2nd_pos.y);
    
    // Now check that both the positions are as expected
    bool success_flag = true;
    
    // Fail the test if the initial position of the bolt is wrong
    if (read_1nd_pos.x != 10 || read_1nd_pos.y != 5) {
        success_flag = false;
    }
    
    // Fail the test if the final position of the bolt is wrong
    if (read_2nd_pos.x != 15 || read_2nd_pos.y != 5) {
        success_flag = false;
    }

    return success_flag;
}
#endif