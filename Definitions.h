#pragma once

const double linearTolerance = 0.018;
const double yawTolerance = 1;
const double deltaTolerance = 0.02;

const double tolerance = 0.01;
const double slowSpeedRange = 10 * tolerance;
const double linearSpeed = 0.3;
const double slowSpeedRatio = 0.1;

#define LINEAR_SPEED 0.12
#define LEFT_ANGULAR_SPEED 0.18
#define RIGHT_ANGULAR_SPEED -0.18

#define LOOK_UP 90
#define LOOK_RIGHT 0
#define LOOK_LEFT 180
#define LOOK_DOWN -90
