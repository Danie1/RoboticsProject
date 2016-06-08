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

Robot::Robot(string ip, int port, double RobotSize) : m_CurrentLocation(0,0,0)
{
	pc = new PlayerClient(ip, port);
	pp = new Position2dProxy(pc);
	lp = new LaserProxy(pc);
	pp->SetMotorEnable(true);
	m_RobotSizeInPixel = RobotSize;
}

void Robot::Read() {
	pc->Read();

	// Update current location:
	m_CurrentLocation = Location(GetX() * m_RobotSizeInPixel, GetY() * m_RobotSizeInPixel, GetYaw());

}

bool Robot::InRadius(Point pnt)
{
	return m_CurrentLocation.GetDistanceFrom(pnt) <= MINIMUM_DISTANCE_FROM_POINT;
}

double Robot::GetX()
{
	return pp->GetXPos();
}

double Robot::GetY()
{
	return pp->GetYPos();
}

double Robot::GetYaw()
{
	return Math::ConvertRadiansToDegrees(pp->GetYaw());
}

void Robot::SetSpeed(double linearSpeed, double angularSpeed) {
	pp->SetSpeed(linearSpeed, angularSpeed);
}

void Robot::SetOdometry(double x, double y, double yaw) {
	pp->SetOdometry(x, y, yaw);
}

Robot::~Robot() {
	delete lp;
	delete pp;
	delete pc;
}

