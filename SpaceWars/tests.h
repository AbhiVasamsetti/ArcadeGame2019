#ifndef TESTS_H
#define TESTS_H

#include "LaserBolt-test.h"
#include "EnemyShipOne-test.h"
#include "Spaceship-test.h"

/**
 * @brief Run all the tests for this program
 *
 * @returns The number of tests that failed
 */
int run_all_tests()
{
    int n_tests_failed = 0; // A log of the number of tests that have failed

    // Run the bolt_test_movement test
    printf("Testing bolt_test_movement...\n");
    bool bolt_test_passed = bolt_test_movement_x();

    // Print out the result of this test
    if (bolt_test_passed) {
        printf("...Passed!\n");
    }
    else {
        printf("...FAILED!\n");
        ++n_tests_failed; // Increment number of failures
    }
    
    // Run the enemy_test_movement_x test
    printf("Testing enemy_test_movement_x...\n");
    bool enemy_test_passed = enemy_test_movement_x();

    // Print out the result of this test
    if (enemy_test_passed) {
        printf("...Passed!\n");
    }
    else {
        printf("...FAILED!\n");
        ++n_tests_failed; // Increment number of failures
    }
    
    // Run the spaceship_test_movement test
    printf("Testing spaceship_movement_y...\n");
    bool spaceship_test_passed = spaceship_test_movement_y();

    // Print out the result of this test
    if (spaceship_test_passed) {
        printf("...Passed!\n");
    }
    else {
        printf("...FAILED!\n");
        ++n_tests_failed; // Increment number of failures
    }
    
    
    // Finish by printing a summary of the tests
    if (n_tests_failed > 0) {
        printf("%d tests FAILED!\n", n_tests_failed);
    }
    else {
        printf("All tests passed!\n");
    }

    return n_tests_failed;
}

#endif