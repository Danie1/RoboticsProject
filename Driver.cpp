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

Driver::Driver(Robot& robot, Localization& loc) : m_robot(robot), m_localization(loc) {

}

double Driver::distance(double x1, double y1, double x2, double y2) {
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

void Driver::MoveToWayPoint(Point pnt)
{
	Point CurrPoint = Point(m_robot.GetX(), m_robot.GetY());
	if (CurrPoint.GetDistanceFrom(pnt) < tolerance)
	{
		printf("next waypoint is close. skipping it\r\n");
		return;
	}
	TurnToPoint(pnt);

	if (!MoveToPoint(pnt))
	{
		return;
	}
}

bool Driver::MoveToPoint(Point pnt) {
	m_robot.Read();

	Point CurrPoint = Point(m_robot.GetX(), m_robot.GetY());
	//cout << "x: " << CurrPoint.GetX() << ", y: " << CurrPoint.GetY() << endl;

#ifdef PARTICLES
	Location oldLocation(m_robot.GetX(), m_robot.GetY(), m_robot.GetYaw());
	int counter = 0;
#endif

	Particle* bestPart;

	while (CurrPoint.GetDistanceFrom(pnt)> slowSpeedRange) {
		m_robot.SetSpeed(linearSpeed, 0);

		m_robot.Read();

#ifdef PARTICLES
		counter++;
		if (counter % 5 == 0)
		{
			m_localization.Update(oldLocation.GetX() - m_robot.GetX(), oldLocation.GetY() - m_robot.GetY(), oldLocation.GetYaw() - m_robot.GetYaw(), m_robot);
			oldLocation.SetX(m_robot.GetX());
			oldLocation.SetY(m_robot.GetY());
			oldLocation.SetYaw(m_robot.GetYaw());
			counter = 0;
		}

		bestPart = m_localization.BestParticle();

		printf("robotLoc (%f,%f,%f). bestPart (%f,%f,%f)\r\n", m_robot.GetX(), m_robot.GetY(), m_robot.GetYaw(),
																bestPart->GetX(), bestPart->GetY(), bestPart->GetYaw());

#endif


		CurrPoint = Point(m_robot.GetX(), m_robot.GetY());
		printf("FullSpeed: Distance from point is: %f - Point(%f, %f) , Dest Point: (%f, %f) \r\n", CurrPoint.GetDistanceFrom(pnt),
																	m_robot.GetX(),
																	m_robot.GetY(),
																	pnt.GetX(),
																	pnt.GetY());
	}

	double OldDistance = -1;
	double NewDistance = -1;

	while (CurrPoint.GetDistanceFrom(pnt) > tolerance) {
		m_robot.SetSpeed(linearSpeed * slowSpeedRatio, 0);

		m_robot.Read();

		OldDistance = CurrPoint.GetDistanceFrom(pnt);
		CurrPoint = Point(m_robot.GetX(), m_robot.GetY());
		NewDistance = CurrPoint.GetDistanceFrom(pnt);

		printf("SlowSpeed: Distance from point is: %f - Point(%f, %f) , Dest Point: (%f, %f) \r\n", CurrPoint.GetDistanceFrom(pnt),
																	m_robot.GetX(),
																	m_robot.GetY(),
																	pnt.GetX(),
																	pnt.GetY());
		if (OldDistance < NewDistance)
		{
			printf("Oops - We moved too far! \r\n");
			m_robot.SetSpeed(0, 0);
			return false;
		}
	}

	m_robot.SetSpeed(0, 0);
	return true;
}

void Driver::TurnToDegree(double degree)
{
	double speed;

	//abort robot to move if Yaw is same
	m_robot.Read();

	double CurrRobotYaw = m_robot.GetYaw();
	if(abs(CurrRobotYaw - degree) < 0.4)
		return;

	double DegreeDelta = fmod((CurrRobotYaw - degree) + 360, 360);

	printf("Curr at: %f, Turning to: %f, by: %f \r\n", CurrRobotYaw, degree, DegreeDelta);

	if (DegreeDelta < 180.0)
		speed = RIGHT_ANGULAR_SPEED;
	else
		speed = LEFT_ANGULAR_SPEED;

	m_robot.SetSpeed(0,speed);

	while(abs(m_robot.GetYaw() - degree) > 4)
	{
		m_robot.Read();
		printf("fast: Curr Robot Yaw: %f || Degree: %f\r\n", m_robot.GetYaw(), degree);
	}

	//slow down before reaching angle target
	m_robot.SetSpeed(0,speed*0.2);
	while(abs(m_robot.GetYaw() - degree) > 0.4)
	{
		m_robot.Read();
		printf("slow: Curr Robot Yaw: %f || Degree: %f\r\n", m_robot.GetYaw(), degree);
	}
	m_robot.SetSpeed(0,0);
}

void Driver::TurnToPoint(Point loc)
{
	printf("1: Turning from: (%f, %f), to: (%f, %f)  \r\n", m_robot.GetX(),
														 m_robot.GetY(),
														 loc.GetX(),
														 loc.GetY());
	TurnToDegree(m_robot.GetCurrentLocation().GetAngleFrom(loc));
	m_robot.Read();

	printf("2: Turning from: (%f, %f), to: (%f, %f)  \r\n", m_robot.GetX(),
														 m_robot.GetY(),
														 loc.GetX(),
														 loc.GetY());
}

Driver::~Driver() {
	// TODO Auto-generated destructor stub
}

