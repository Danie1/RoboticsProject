/*
 * Utils.h
 *
 *  Created on: Apr 15, 2016
 *      Author: Or Gafni and Daniel Saad
 */

#ifndef UTILS_H_
#define UTILS_H_

class Utils {
public:
	static bool getParamsFromFile(const char* paramsFileName,
								  char* o_inputMapFileName,
								  char* o_outputMapFileName,
								  double* o_mapResolution,
								  double* o_robotSize);
};

#endif /* UTILS_H_ */
