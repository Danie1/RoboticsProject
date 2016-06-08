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
	LaserProxy *lp;
	double m_RobotSizeInPixel;
public:
	Robot(string ip, int port, double RobotSize);
	void Read();
	double GetX();
	double GetY();
	double GetYaw();
	void SetOdometry(double x, double y, double yaw);
	void SetSpeed(double linearSpeed, double angularSpeed);
	bool InRadius(Point pnt);
	virtual ~Robot();

	Location GetCurrentLocation() {
		return m_CurrentLocation;
	}

	Location m_CurrentLocation;
};

#endif /* ROBOT_H_ */
