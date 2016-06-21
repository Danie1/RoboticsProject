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

int Driver::calcYawBetweenPoints(Point src, Point dst)
{
	dst.SetY(2* src.GetY() - dst.GetY());
	Point newDst(dst.GetX()-src.GetX(), dst.GetY() - src.GetY());

	cout << "newDst = x: " << newDst.GetX() << ", y: " << newDst.GetY() << endl;
	double angle = atan(double(newDst.GetY()) / double(newDst.GetX()));
	cout << "angle(rad): " << angle << endl;
	cout << "angle(deg): " << Math::ConvertRadiansToDegrees(angle) << endl;

	return Math::ConvertRadiansToDegrees(angle);

//	double dot = src.GetX()*dst.GetX() + src.GetY()*dst.GetY(); //      # dot product
//	double det = src.GetX()*dst.GetY() - src.GetY()*dst.GetX(); //      # determinant
//	double angle = atan2(det, dot);  //# atan2(y, x) or atan2(sin, cos)
//
//	cout << "angle: " << angle << endl;
//	return Math::ConvertRadiansToDegrees(angle);
//	double deltaX = dst.GetX() - src.GetX();
//	double deltaY = dst.GetY() - src.GetY();
//	double goalYaw = atan2(deltaY, deltaX);

//	return Math::ConvertRadiansToDegrees(goalYaw);

//	return Math::ConvertRadiansToDegrees(atan((dst.GetX() - src.GetX())/(dst.GetY() - src.GetY())));
}

void Driver::moveToNextWaypoint(Point pnt) {
	m_robot.Read();

	Point CurrPoint(m_robot.GetX(), m_robot.GetY());
	cout << "CurrPoint - x: " << CurrPoint.GetX() << ", y: " << CurrPoint.GetY() << ", RoboYaw: " << m_robot.GetYaw()  << endl;
	cout << "DestPnt - x: " << pnt.GetX() << ", y: " << pnt.GetY() << endl;

	cout << "distance between currPoint and DestPnt: " << CurrPoint.GetDistanceFrom(pnt) << endl;
	if (CurrPoint.GetDistanceFrom(pnt) < tolerance)
	{
		printf("robot very close to dst point, no need to move\n");
		return;
	}

	int yawToPoint = calcYawBetweenPoints(CurrPoint, pnt);
	cout << "YawToPoint: " << yawToPoint << endl;
	Location locToTurnTo(0,0, yawToPoint);
	TurnToPoint(locToTurnTo);
	cout << "Robot yaw after turn: " << m_robot.GetYaw() << endl;

	while (CurrPoint.GetDistanceFrom(pnt)> slowSpeedRange) {
		m_robot.SetSpeed(linearSpeed, 0);

		m_robot.Read();

		Point CurrPoint(m_robot.GetX(), m_robot.GetY());
		cout << "Going full speed: x: " << CurrPoint.GetX() << ", y: " << CurrPoint.GetY() << endl;
	}

	while (CurrPoint.GetDistanceFrom(pnt) > tolerance) {
		m_robot.SetSpeed(linearSpeed * slowSpeedRatio, 0);

		m_robot.Read();

		Point CurrPoint(m_robot.GetX(), m_robot.GetY());
		cout << "Going slow speed: x: " << CurrPoint.GetX() << ", y: " << CurrPoint.GetY() << endl;
	}

	m_robot.SetSpeed(0, 0);
}

void Driver::TurnToPoint(Location loc)
{
		double speed;
		//abort robot to move if Yaw is same
		m_robot.Read();

		double CurrRobotYaw = m_robot.GetYaw();
		printf("CurrRobotYaw = %f\n", CurrRobotYaw);
		printf("loc.GetYaw() = %d\n", loc.GetYaw());
		if(abs(CurrRobotYaw - loc.GetYaw()) < yawTolerance)
		{
			printf("robot aiming to good direction\n");
			return;
		}

		if (CurrRobotYaw >= 0 && CurrRobotYaw <= 180)
		{
			if (loc.GetYaw() > CurrRobotYaw && loc.GetYaw() < (CurrRobotYaw + 180))
			{
				printf("robot need to turn left\n");
				speed = LEFT_ANGULAR_SPEED;
			}
			else
			{
				printf("robot need to turn right\n");
				speed = RIGHT_ANGULAR_SPEED;
			}
		}
		else
		{
			if (loc.GetYaw() > (int(CurrRobotYaw) + 180) % 360 && loc.GetYaw() < CurrRobotYaw)
			{
				printf("robot need to turn right\n");
				speed = RIGHT_ANGULAR_SPEED;
			}
			else
			{
				printf("robot need to turn left\n");
				speed = LEFT_ANGULAR_SPEED;
			}
		}
//		if (CurrRobotYaw - loc.GetYaw() % 360 < 180)
//		{
//			printf("robot need to turn right\n");
//			speed = RIGHT_ANGULAR_SPEED;
//		}
//		else
//		{
//			printf("robot need to turn left\n");
//			speed = LEFT_ANGULAR_SPEED;
//		}

		m_robot.SetSpeed(0,speed);

		while(abs(m_robot.GetYaw() - loc.GetYaw()) > yawTolerance * 2)
		{
			cout << "1: Curr Robot Yaw: " << m_robot.GetYaw() << endl;
			m_robot.Read();
		}
		cout << "after first while: Curr Robot Yaw: " << m_robot.GetYaw() << endl;

		//slow down before reaching angle target
		m_robot.SetSpeed(0,speed*0.25);
		while(abs(m_robot.GetYaw() - loc.GetYaw()) > yawTolerance / 2)
		{
			cout << "2: Curr Robot Yaw: " << m_robot.GetYaw() << endl;
			m_robot.Read();
		}
		cout << "after second while: Curr Robot Yaw: " << m_robot.GetYaw() << endl;
		m_robot.SetSpeed(0,0);

}

Driver::~Driver() {
	// TODO Auto-generated destructor stub
}

