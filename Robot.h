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

class Robot {
private:
	PlayerClient *pc;
	Position2dProxy *pp;
	LaserProxy *lp;
public:
	Robot(string ip, int port);
	void read();
	double getX();
	double getY();
	void setOdometry(double x, double y, double yaw);
	void setSpeed(double linearSpeed, double angularSpeed);
	virtual ~Robot();
};

#endif /* ROBOT_H_ */
