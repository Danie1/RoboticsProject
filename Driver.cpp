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

Driver::Driver(Robot& robot) : m_robot(robot) {

}

double Driver::distance(double x1, double y1, double x2, double y2) {
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

void Driver::moveToNextWaypoint(Point pnt) {
	m_robot.Read();

	Point CurrPoint = Point(m_robot.GetX(), m_robot.GetY());
	//cout << "x: " << CurrPoint.GetX() << ", y: " << CurrPoint.GetY() << endl;

	while (CurrPoint.GetDistanceFrom(pnt)> slowSpeedRange) {
		m_robot.SetSpeed(linearSpeed, 0);

		m_robot.Read();

		CurrPoint = Point(m_robot.GetX(), m_robot.GetY());
		printf("Distance from point is: %f \r\n", CurrPoint.GetDistanceFrom(pnt));
	}

	while (CurrPoint.GetDistanceFrom(pnt) > tolerance) {
		m_robot.SetSpeed(linearSpeed * slowSpeedRatio, 0);

		m_robot.Read();

		CurrPoint = Point(m_robot.GetX(), m_robot.GetY());
		printf("Distance from point is: %f \r\n", CurrPoint.GetDistanceFrom(pnt));
	}

	m_robot.SetSpeed(0, 0);
}

void Driver::TurnByDegree(double degree)
{
	double speed;

	//abort robot to move if Yaw is same
	m_robot.Read();

	double CurrRobotYaw = m_robot.GetYaw();
	if(abs(CurrRobotYaw - degree) < yawTolerance)
		return;

	double DegreeDelta = fmod((CurrRobotYaw - degree) + 360, 360);

	printf("Curr at: %f, Turning to: %f, by: %f \r\n", CurrRobotYaw, degree, DegreeDelta);

	if (DegreeDelta < 180.0)
		speed = RIGHT_ANGULAR_SPEED;
	else
		speed = LEFT_ANGULAR_SPEED;

	m_robot.SetSpeed(0,speed);

	while(abs(m_robot.GetYaw() - degree) > yawTolerance)
	{
		cout << "Curr Robot Yaw: " << m_robot.GetYaw() << endl;
		m_robot.Read();
	}

	//slow down before reaching angle target
	m_robot.SetSpeed(0,speed*0.5);
	while(abs(m_robot.GetYaw() - degree) < yawTolerance)
		m_robot.Read();
	m_robot.SetSpeed(0,0);
}

void Driver::TurnToPoint(Point loc)
{
	//printf("Turning by: %f \r\n", m_robot.GetCurrentLocation().GetAngleFrom(loc));
	TurnByDegree(m_robot.GetCurrentLocation().GetAngleFrom(loc));
}

Driver::~Driver() {
	// TODO Auto-generated destructor stub
}

