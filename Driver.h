/*
 * Driver.h
 *
 *  Created on: Apr 12, 2016
 *      Author: colman
 */

#ifndef DRIVER_H_
#define DRIVER_H_
#include "Robot.h"

class Driver {
private:
	const double tolerance = 0.01;
	const double slowSpeedRange = 10 * tolerance;
	const double linearSpeed = 0.8;
	const double slowSpeedRatio = 0.1;

	Robot *robot;
	double distance(double x1, double y1, double x2, double y2);

public:
	Driver(Robot *robot);
	void moveToNextWaypoint(double x, double y);
	virtual ~Driver();
};

#endif /* DRIVER_H_ */
