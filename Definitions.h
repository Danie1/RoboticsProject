#pragma once

const double yawTolerance = 1;
const double slowSpeedYawRange = 0.4 * yawTolerance;

const double tolerance = 0.5;
const double slowSpeedRange = 5 * tolerance;
const double linearSpeed = 0.3;
const double slowSpeedRatio = 0.1;

#define LEFT_ANGULAR_SPEED 0.10
#define RIGHT_ANGULAR_SPEED -0.10

#define LOOK_UP 90
#define LOOK_RIGHT 0
#define LOOK_LEFT 180
#define LOOK_DOWN -90

///////////////////////////////////////
#define MAX_PARTICLES_COUNT 0
#define LOW_BELIEF_MIN 0.5
#define HIGH_BELIEF_MIN 0.7
#define BELIEF_MAGIC_NUMBER 0.9

// laser
#define LASER_COUNT 666
#define LASER_SCOPE 240
#define OBSTACLE_DISTANCE 4
