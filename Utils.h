/*
 * Utils.h
 *
 *  Created on: Apr 15, 2016
 *      Author: Or Gafni and Daniel Saad
 */

#ifndef UTILS_H_
#define UTILS_H_

#include "CommonStructs.h"

class Utils {
public:
	static bool getParamsFromFile(const char* paramsFileName,
								  char* o_inputMapFileName,
								  Location& StartLocation,
								  Point& EndPoint,
								  int& o_robotSize,
								  double& o_mapResolution,
								  double& o_gridResolution);
};

#endif /* UTILS_H_ */
