/*
 * Driver.cpp
 *
 *  Created on: Apr 12, 2016
 *      Author: colman
 */

#include "Driver.h"
#include <cmath>

Driver::Driver(Robot *robot) : robot(robot) {

}

double Driver::distance(double x1, double y1, double x2, double y2) {
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

void Driver::moveToNextWaypoint(double x, double y) {
	robot->read();
	double currX = robot->getX();
	double currY = robot->getY();
	cout << "x: " << currX << ", y: " << currY << endl;

	while (distance(currX, currY, x, y) > slowSpeedRange) {
		robot->setSpeed(linearSpeed, 0);

		robot->read();
		currX = robot->getX();
		currY = robot->getY();
		cout << "x: " << currX << ", y: " << currY << endl;
	}

	while (distance(currX, currY, x, y) > tolerance) {
		robot->setSpeed(linearSpeed * slowSpeedRatio, 0);

		robot->read();
		currX = robot->getX();
		currY = robot->getY();
		cout << "x: " << currX << ", y: " << currY << endl;
	}

	robot->setSpeed(0, 0);
}

Driver::~Driver() {
	// TODO Auto-generated destructor stub
}

