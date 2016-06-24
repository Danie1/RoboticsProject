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
	void TurnToDegree(double degree);

public:
	Driver(Robot& robot);
	bool MoveToPoint(Point pnt);
	void TurnToPoint(Point loc);
	void MoveToWayPoint(Point pnt);
	virtual ~Driver();
};

#endif /* DRIVER_H_ */
