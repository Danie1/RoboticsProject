/*
 * Driver.h
 *
 *  Created on: Apr 12, 2016
 *      Author: colman
 */

#ifndef DRIVER_H_
#define DRIVER_H_
#include "Robot.h"
#include "CommonStructs.h"

class Driver {
private:
	Robot& m_robot;
	double distance(double x1, double y1, double x2, double y2);
	void TurnByDegree(double degree);

public:
	Driver(Robot& robot);
	void moveToNextWaypoint(Point pnt);
	void TurnToPoint(Point loc);
	virtual ~Driver();
};

#endif /* DRIVER_H_ */
