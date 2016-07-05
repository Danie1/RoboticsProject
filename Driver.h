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
#include "LocalizationManager.h"

class Driver {
private:
	Robot& m_robot;
	Localization& m_localization;
	double distance(double x1, double y1, double x2, double y2);
	void TurnToDegree(double degree);

public:
	Driver(Robot& robot, Localization& loc);
	bool MoveToPoint(Point pnt, Point CurrWayPoint);
	void TurnToPoint(Point loc);
	void MoveToWayPoint(Point pnt, Point CurrWayPoint);
	virtual ~Driver();
};

#endif /* DRIVER_H_ */
