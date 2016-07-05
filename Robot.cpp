/*
 * Robot.cpp
 *
 *  Created on: Apr 12, 2016
 *      Author: colman
 */

#include "Robot.h"
#include "Math.h"
#include "Definitions.h"

#define MINIMUM_DISTANCE_FROM_POINT 10

Robot::Robot(string ip, int port, Location StartLocation) : m_CurrentLocation(0,0,0),
															m_StartLocation(StartLocation)
{
	pc = new PlayerClient(ip, port);
	pp = new Position2dProxy(pc);
	lp = new LaserProxy(pc);
	pp->SetMotorEnable(true);
}

void Robot::Read() {
	// DanielS: Reading twice due to some freakishly odd bug in PlayerStage
	for (int i = 0; i< 15; i++)
		pc->Read();

	// Update current location:
	m_CurrentLocation = Location(GetRobotX(),
								 GetRobotY(),
								 GetRobotYaw());

}

bool Robot::InRadius(Point pnt)
{
	double distanceFromPnt = GetCurrentLocation().GetDistanceFrom(pnt);

	return GetCurrentLocation().GetDistanceFrom(pnt) <= tolerance;
}

double Robot::GetYaw()
{
	return m_CurrentLocation.GetYaw();
//	return Math::ConvertRadiansToDegrees(pp->GetYaw());
}

void Robot::SetSpeed(double linearSpeed, double angularSpeed)
{
	pp->SetSpeed(linearSpeed, angularSpeed);
}

void Robot::SetOdometry(double x, double y, double yaw)
{
	for (int i = 0; i< 15; i++)
		pp->SetOdometry(x, y, Math::ConvertDegreesToRadians(yaw));
}

Robot::~Robot()
{
	delete lp;
	delete pp;
	delete pc;
}

double Robot::GetX()
{
	const int GRAPH_RESOLUTION = 10;

	double NewX = m_CurrentLocation.GetX() - (2 * (m_CurrentLocation.GetX() - m_StartLocation.GetX()));

	double retVal = m_StartLocation.GetX() + (NewX - m_StartLocation.GetX()) * GRAPH_RESOLUTION;

	return retVal;
}

double Robot::GetY()
{
	const int GRAPH_RESOLUTION = 10;

	double retVal = m_StartLocation.GetY() + (GetRobotY() - m_StartLocation.GetY()) * GRAPH_RESOLUTION;

	return retVal;
}

double Robot::GetDistanceFromLaser(int index)
{
	return lp->GetRange(index);
}

//// Private accessors
double Robot::GetRobotX()
{
	return pp->GetXPos();
}

double Robot::GetRobotY()
{
	return pp->GetYPos();
}

double Robot::GetRobotYaw()
{
	return Math::ConvertRadiansToDegrees(pp->GetYaw());
}

