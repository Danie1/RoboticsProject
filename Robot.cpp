/*
 * Robot.cpp
 *
 *  Created on: Apr 12, 2016
 *      Author: colman
 */

#include "Robot.h"

Robot::Robot(string ip, int port) {
	pc = new PlayerClient(ip, port);
	pp = new Position2dProxy(pc);
	lp = new LaserProxy(pc);
	pp->SetMotorEnable(true);
}

void Robot::read() {
	pc->Read();
}

double Robot::getX() {
	return pp->GetXPos();
}

double Robot::getY() {
	return pp->GetYPos();
}

void Robot::setSpeed(double linearSpeed, double angularSpeed) {
	pp->SetSpeed(linearSpeed, angularSpeed);
}

void Robot::setOdometry(double x, double y, double yaw) {
	pp->SetOdometry(x, y, yaw);
}

Robot::~Robot() {
	delete lp;
	delete pp;
	delete pc;
}

