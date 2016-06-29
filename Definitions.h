#pragma once

/**
 * General configuration
 */
#define MAX_FILE_PATH (30)
#define PARAMS_FILE_NAME "parameters.txt"

/**
 * Robot rotation
 */
// Used for rotating the robot.
const double yawTolerance = 4;
const double slowSpeedYawRange = 0.1 * yawTolerance;

// The speed of the robot when rotating
#define LEFT_ANGULAR_SPEED 0.10
#define RIGHT_ANGULAR_SPEED -0.10

/**
 * Robot movement
 */
// Used for moving the robot.
const double tolerance = 0.5;
const double slowSpeedRange = 5 * tolerance;
const double linearSpeed = 0.3;
const double slowSpeedRatio = 0.1;

/**
 * Particles
 */
// Turn particles on/off (comment to remove particles)
#define ENABLE_PARTICLES

// General particles configuration
#ifdef ENABLE_PARTICLES
#define MAX_PARTICLES_COUNT 100
#else
#define MAX_PARTICLES_COUNT 0
#endif

#define LOW_BELIEF_MIN 0.5
#define HIGH_BELIEF_MIN 0.7

// Laser configuration
#define LASER_COUNT 666
#define LASER_SCOPE 240
#define OBSTACLE_DISTANCE 4

