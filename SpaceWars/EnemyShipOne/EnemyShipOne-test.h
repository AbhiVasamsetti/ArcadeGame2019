#ifndef ENEMY_SHIP_ONE_TEST_H
#define ENEMY_SHIP_ONE_TEST_H

/**
 * \brief Checks that the enemy ship moves in the intended x-direction to a position set by the user
 * 
 * \returns True if tests are passed.
 */
bool enemy_test_movement_x()
{
    // Initialise the enemy object with a speed of 5
    EnemyShipOne enemy;
    enemy.init(0,0,5);

    // Set the initial position to 10, 5
    Vector2D initial_enemy_pos = {10, 5};
    enemy.set_pos(initial_enemy_pos);

    // Read the 1st position of the enemy
    Vector2D read_1st_pos = enemy.get_pos();
    printf("%f, %f\n", read_1st_pos.x, read_2nd_pos.y);

    // Set the x velocity to 4 (y velocity does not update for the enemy, so keep the same as the enemy y position)
    Vector2D velocity = {5, 5};
    enemy.set_velocity(velocity);

    // Update the position of the enemy
    enemy.update();

    // Read the 2nd position of the enemy
    Vector2D read_2nd_pos = enemy.get_pos();
    printf("%f, %f\n", read_2nd_pos.x, read_2nd_pos.y);
    
    // Now check that both the positions are as expected
    bool success_flag = true;
    
    // Fail the test if the initial position of the enemy is wrong
    if (read_2nd_pos.x != 10 || read_2nd_pos.y != 5) {
        success_flag = false;
    }
    
    // Fail the test if the final position of the enemy is wrong
    if (read_2nd_pos.x != 5 || read_2nd_pos.y != 5) {
        success_flag = false;
    }

    return success_flag;
}
#endif