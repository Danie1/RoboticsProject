/*
 * Driver.cpp
 *
 *  Created on: Apr 12, 2016
 *      Author: colman
 */

#include "Driver.h"
#include <cmath>
#include "Math.h"
#include "Definitions.h"

Driver::Driver(Robot *robot) : robot(robot) {

}

double Driver::distance(double x1, double y1, double x2, double y2) {
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

void Driver::moveToNextWaypoint(Point pnt) {
	robot->Read();

	Point CurrPoint(robot->GetX(), robot->GetY());
	cout << "x: " << CurrPoint.GetX() << ", y: " << CurrPoint.GetY() << endl;

	while (CurrPoint.GetDistanceFrom(pnt)> slowSpeedRange) {
		robot->SetSpeed(linearSpeed, 0);

		robot->Read();

		Point CurrPoint(robot->GetX(), robot->GetY());
		cout << "x: " << CurrPoint.GetX() << ", y: " << CurrPoint.GetY() << endl;
	}

	while (CurrPoint.GetDistanceFrom(pnt) > tolerance) {
		robot->SetSpeed(linearSpeed * slowSpeedRatio, 0);

		robot->Read();

		Point CurrPoint(robot->GetX(), robot->GetY());
		cout << "x: " << CurrPoint.GetX() << ", y: " << CurrPoint.GetY() << endl;
	}

	robot->SetSpeed(0, 0);
}

void Driver::TurnToPoint(Location loc)
{
		double speed;
		//abort robot to move if Yaw is same
		robot->Read();

		double CurrRobotYaw = robot->GetYaw();
		if(abs(CurrRobotYaw - loc.GetYaw()) < yawTolerance)
			return;

		if (CurrRobotYaw - loc.GetYaw() % 360 < 180)
			speed = RIGHT_ANGULAR_SPEED;
		else
			speed = LEFT_ANGULAR_SPEED;

		cout << "11Curr Robot Yaw: " << robot->GetYaw() << endl;
		robot->SetSpeed(0,speed);
		robot->Read();
		cout << "22Curr Robot Yaw: " << robot->GetYaw() << endl;

		while(abs(robot->GetYaw() - loc.GetYaw()) > yawTolerance)
		{
			cout << "Curr Robot Yaw: " << robot->GetYaw() << endl;
			robot->Read();
		}

		//slow down before reaching angle target
		robot->SetSpeed(0,speed*0.5);
		while(abs(robot->GetYaw() - loc.GetYaw()) < yawTolerance)
			robot->Read();
		robot->SetSpeed(0,0);

}

Driver::~Driver() {
	// TODO Auto-generated destructor stub
}

