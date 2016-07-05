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

void Driver::MoveToWayPoint(Point pnt, Point CurrWayPoint)
{
	Point CurrPoint = Point(m_robot.GetX(), m_robot.GetY());
	if (CurrPoint.GetDistanceFrom(pnt) < tolerance)
	{
		MOVING_MSG("Next waypoint is close. Skipping it.\r\n");
		return;
	}
	TurnToPoint(pnt);

	if (!MoveToPoint(pnt, CurrWayPoint))
	{
		return;
	}
}

bool Driver::MoveToPoint(Point pnt, Point CurrWayPoint) {
	m_robot.Read();

	Point CurrPoint = Point(m_robot.GetX(), m_robot.GetY());
	//cout << "x: " << CurrPoint.GetX() << ", y: " << CurrPoint.GetY() << endl;

#ifdef ENABLE_PARTICLES
	Location oldLocation = Location(m_robot.GetX(),
									m_robot.GetY(),
									m_robot.GetYaw());
#endif

	Particle* bestPart;

	double speedStep = linearSpeed / 10;
	double speedToSet = 0;

	double slowingDownRange = CurrPoint.GetDistanceFrom(pnt) / 3.0;
	// acceleartion to full speed loop
	while (CurrPoint.GetDistanceFrom(pnt) > slowingDownRange &&
		   CurrPoint.GetDistanceFrom(pnt) > slowSpeedRange + slowingDownDist)
	{
		if (speedToSet < linearSpeed)
		{
			speedToSet += speedStep;
		}
		m_robot.SetSpeed(speedToSet, 0);

		m_robot.Read();

#ifdef ENABLE_PARTICLES
		m_localization.Update(m_robot.GetX() - oldLocation.GetX(),
							  m_robot.GetY() - oldLocation.GetY(),
							  m_robot.GetYaw() - oldLocation.GetYaw(), m_robot);
		oldLocation.SetX(m_robot.GetX());
		oldLocation.SetY(m_robot.GetY());
		oldLocation.SetYaw(m_robot.GetYaw());

	bestPart = m_localization.BestParticle();

	MOVING_MSG("robotLoc (%f,%f,%f). bestPart (%f,%f,%f)\r\n", m_robot.GetX(), m_robot.GetY(), m_robot.GetYaw(),
															bestPart->GetX(), bestPart->GetY(), bestPart->GetYaw());


#endif


		CurrPoint = Point(m_robot.GetX(), m_robot.GetY());
		MOVING_MSG("FullSpeed (%f): Distance from point is: %f - Point(%f, %f) , Dest Point: (%f, %f) \r\n",
																	speedToSet,
																	CurrPoint.GetDistanceFrom(pnt),
																	m_robot.GetX(),
																	m_robot.GetY(),
																	pnt.GetX(),
																	pnt.GetY());
	}

	speedToSet = MAX(speedToSet, (linearSpeed * slowSpeedRatio));
	// slowing down loop
	while (CurrPoint.GetDistanceFrom(pnt) > slowSpeedRange)
	{
		if (speedToSet > linearSpeed * slowSpeedRatio)
		{
			speedToSet -= speedStep;
		}
		m_robot.SetSpeed(speedToSet, 0);
		m_robot.Read();

#ifdef ENABLE_PARTICLES
		m_localization.Update(m_robot.GetX() - oldLocation.GetX(),
							  m_robot.GetY() - oldLocation.GetY(),
							  m_robot.GetYaw() - oldLocation.GetYaw(), m_robot);
		oldLocation.SetX(m_robot.GetX());
		oldLocation.SetY(m_robot.GetY());
		oldLocation.SetYaw(m_robot.GetYaw());

	bestPart = m_localization.BestParticle();

	MOVING_MSG("robotLoc (%f,%f,%f). bestPart (%f,%f,%f)\r\n", m_robot.GetX(), m_robot.GetY(), m_robot.GetYaw(),
															bestPart->GetX(), bestPart->GetY(), bestPart->GetYaw());


#endif

		CurrPoint = Point(m_robot.GetX(), m_robot.GetY());

		MOVING_MSG("slowing down speed (%f): Distance from point is: %f - Point(%f, %f) , Dest Point: (%f, %f) \r\n",
																	speedToSet,
																	CurrPoint.GetDistanceFrom(pnt),
																	m_robot.GetX(),
																	m_robot.GetY(),
																	pnt.GetX(),
																	pnt.GetY());
	}

	double OldDistance = -1;
	double NewDistance = -1;

	// slow speed until dest
	while (CurrPoint.GetDistanceFrom(pnt) > tolerance)
	{
		m_robot.SetSpeed(linearSpeed * slowSpeedRatio, 0);

		m_robot.Read();

		OldDistance = CurrPoint.GetDistanceFrom(pnt);
		CurrPoint = Point(m_robot.GetX(), m_robot.GetY());


#ifdef ENABLE_PARTICLES
		m_localization.Update(m_robot.GetX() - oldLocation.GetX(),
							  m_robot.GetY() - oldLocation.GetY(),
							  m_robot.GetYaw() - oldLocation.GetYaw(), m_robot);
		oldLocation.SetX(m_robot.GetX());
		oldLocation.SetY(m_robot.GetY());
		oldLocation.SetYaw(m_robot.GetYaw());

	bestPart = m_localization.BestParticle();

	MOVING_MSG("robotLoc (%f,%f,%f). bestPart (%f,%f,%f)\r\n", m_robot.GetX(), m_robot.GetY(), m_robot.GetYaw(),
															bestPart->GetX(), bestPart->GetY(), bestPart->GetYaw());


#endif

		NewDistance = CurrPoint.GetDistanceFrom(pnt);

		MOVING_MSG("SlowSpeed (%f): Distance from point is: %f - Point(%f, %f) , Dest Point: (%f, %f) \r\n",
																	linearSpeed * slowSpeedRatio,
																	CurrPoint.GetDistanceFrom(pnt),
																	m_robot.GetX(),
																	m_robot.GetY(),
																	pnt.GetX(),
																	pnt.GetY());
		if (OldDistance < NewDistance)
		{
			MOVING_MSG("Oops - We moved too far! \r\n");
			m_robot.SetSpeed(0, 0);
			return false;
		}
	}

	m_robot.SetSpeed(0, 0);
	return true;
}

void Driver::TurnToDegree(double degree)
{
	auto IsBeyondDegree = [&](double degree, double speed)
							{
								// Check if turning left
								if (speed > 0)
								{
									if (m_robot.GetYaw() > degree)
									{
										return true;
									}
								}
								else
								{
									if (m_robot.GetYaw() < degree)
									{
										return true;
									}
								}

								return false;
							};

	double speed;

	//abort robot to move if Yaw is same
	m_robot.Read();

	double CurrRobotYaw = m_robot.GetYaw();
	if(abs(CurrRobotYaw - degree) < slowSpeedYawRange)
		return;

	double DegreeDelta = fmod((CurrRobotYaw - degree) + 360, 360);

	// Check whether to turn left or right
	if (DegreeDelta < 180.0)
	{
		speed = RIGHT_ANGULAR_SPEED;
		TURNING_MSG("Curr at: %f, Turning to: %f, by: %f \r\n", CurrRobotYaw, degree, DegreeDelta);
	}
	else
	{
		speed = LEFT_ANGULAR_SPEED;
		TURNING_MSG("Curr at: %f, Turning to: %f, by: %f \r\n", CurrRobotYaw, degree, 360 - DegreeDelta);
	}

	m_robot.SetSpeed(0,speed);

	while(abs(m_robot.GetYaw() - degree) > yawTolerance && !IsBeyondDegree(degree, speed))
	{
		m_robot.Read();
		TURNING_MSG("fast: Curr Robot Yaw: %f || Degree: %f\r\n", m_robot.GetYaw(), degree);
	}

	//slow down before reaching angle target
	m_robot.SetSpeed(0,speed*0.2);
	while(abs(m_robot.GetYaw() - degree) > slowSpeedYawRange && !IsBeyondDegree(degree, speed))
	{
		m_robot.Read();
		TURNING_MSG("slow: Curr Robot Yaw: %f || Degree: %f\r\n", m_robot.GetYaw(), degree);
	}

	m_robot.SetSpeed(0,0);
}

void Driver::TurnToPoint(Point loc)
{
	printf("Turning from: (%f, %f), to: (%f, %f)  \r\n", m_robot.GetX(),
														 m_robot.GetY(),
														 loc.GetX(),
														 loc.GetY());
	TurnToDegree(m_robot.GetCurrentLocation().GetAngleFrom(loc));
}

Driver::~Driver() {
	// TODO Auto-generated destructor stub
}

