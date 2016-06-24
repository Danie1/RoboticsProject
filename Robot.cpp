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

Robot::Robot(string ip, int port, double RobotSize, Point StartPoint) : m_CurrentLocation(0,0,0),
																		m_StartPoint(StartPoint)
{
	pc = new PlayerClient(ip, port);
	pp = new Position2dProxy(pc);
	lp = new LaserProxy(pc);
	pp->SetMotorEnable(true);
	m_RobotSizeInPixel = RobotSize;
}

void Robot::Read() {
	// DanielS: Reading twice due to some freakishly odd bug in PlayerStage
	pc->Read();
	pc->Read();
	pc->Read();
	pc->Read();
	pc->Read();

	// Update current location:
	m_CurrentLocation = Location(GetRobotX(),
								 GetRobotY(),
								 Math::ConvertRadiansToDegrees(GetYaw()));

}

bool Robot::InRadius(Point pnt)
{
	double distanceFromPnt = GetCurrentLocation().GetDistanceFrom(pnt);

	printf("pnt (%f, %f) distance form currLocation (%f , %f) is %f. ok is %f\n", pnt.GetX(), pnt.GetY(), GetCurrentLocation().GetX(), GetCurrentLocation().GetY(), distanceFromPnt, tolerance);
	return GetCurrentLocation().GetDistanceFrom(pnt) <= tolerance;
//	return m_CurrentLocation.GetDistanceFrom(pnt) <= MINIMUM_DISTANCE_FROM_POINT;
}

double Robot::GetYaw()
{
	return Math::ConvertRadiansToDegrees(pp->GetYaw());
}

void Robot::SetSpeed(double linearSpeed, double angularSpeed) {
	pp->SetSpeed(linearSpeed, angularSpeed);
}

void Robot::SetOdometry(double x, double y, double yaw) {
	for (int i = 0; i< 15; i++)
		pp->SetOdometry(x, y, Math::ConvertDegreesToRadians(yaw));
}

Robot::~Robot() {
	delete lp;
	delete pp;
	delete pc;
}

double Robot::GetX()
{
	const int GRAPH_RESOLUTION = 10;

	double NewX = m_CurrentLocation.GetX() - (2 * (m_CurrentLocation.GetX() - m_StartPoint.GetX()));

	double retVal = m_StartPoint.GetX() + (NewX - m_StartPoint.GetX()) * GRAPH_RESOLUTION;

	return retVal;
}

double Robot::GetY()
{
	const int GRAPH_RESOLUTION = 10;

	return m_StartPoint.GetY() + (GetRobotY() - m_StartPoint.GetY()) * GRAPH_RESOLUTION;
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

