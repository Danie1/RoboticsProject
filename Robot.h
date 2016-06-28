/*
 * Robot.h
 *
 *  Created on: Apr 12, 2016
 *      Author: colman
 */

#ifndef ROBOT_H_
#define ROBOT_H_

#include <libplayerc++/playerc++.h>
#include <string>
using namespace PlayerCc;
using namespace std;

#include "CommonStructs.h"

class Robot {
private:
	PlayerClient *pc;
	Position2dProxy *pp;
	double m_RobotSizeInPixel;
public:
	LaserProxy *lp;

public:
	Robot(string ip, int port, double RobotSize, Point StartPoint);
	void Read();
	double GetX();
	double GetY();
	double GetYaw();
	void SetOdometry(double x, double y, double yaw);
	double GetDistanceFromLaser(int index);
	void SetSpeed(double linearSpeed, double angularSpeed);
	bool InRadius(Point pnt);
	virtual ~Robot();

	Location GetCurrentLocation() {
		return Location(GetX(), GetY(), GetYaw());
	}

private:
	double GetRobotX();
	double GetRobotY();

	Location m_CurrentLocation;
	Point m_StartPoint;
};

#endif /* ROBOT_H_ */
