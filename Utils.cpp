/*
 * Utils.cpp
 *
 *  Created on: Apr 15, 2016
 *      Author: Or Gafni and Daniel Saad
 */

#include "Utils.h"

#include <stdlib.h>
#include <iostream>
#include <fstream>
using std::ifstream;

#include <cstring>

using namespace std;


#define INPUT_MAP_PARAM ("map:")
#define START_LOCATION_PARAM ("startLocation:")
#define GOAL_LOCATION_PARAM ("goal:")
#define ROBOT_SIZE_PARAM ("robotSize:")
#define MAP_RESO_PARAM ("MapResolutionCM:")
#define GRID_RESO_PARAM ("GridResolutionCM:")

bool Utils::getParamsFromFile(const char* paramsFileName,
							  char* o_inputMapFileName,
							  Location& StartLocation,
							  Point& EndPoint,
							  int& o_robotSize,
							  double& o_mapResolution,
							  double& o_gridResolution)
{
	const int MAX_CHARS_PER_LINE = 512;
	const int MAX_TOKENS_PER_LINE = 20;
	const char* const DELIMITER = " ";

	// create a file-reading object
	ifstream paramsFile;
	paramsFile.open(paramsFileName); // open a file
	if (!paramsFile.good())
	{
		printf("failed to open file\n");
		return false; // exit if file not found
	}

	// read each line of the file
	while (!paramsFile.eof())
	{
		// read an entire line into memory
		char buf[MAX_CHARS_PER_LINE];
		paramsFile.getline(buf, MAX_CHARS_PER_LINE);

		// parse the line into blank-delimited tokens
		int n = 0; // a for-loop index

		// array to store memory addresses of the tokens in buf
		const char* token[MAX_TOKENS_PER_LINE] = {}; // initialize to 0

		// parse the line
		token[0] = strtok(buf, DELIMITER); // first token
		if (token[0]) // zero if line is blank
		{
			for (n = 1; n < MAX_TOKENS_PER_LINE; n++)
			{
				token[n] = strtok(0, DELIMITER); // subsequent tokens
				if (!token[n]) break; // no more tokens
			}
		}

		if (token[0])
		{
			if (strcmp(token[0], INPUT_MAP_PARAM) == 0)
			{
				strcpy(o_inputMapFileName, token[1]);
			}
			else if (strcmp(token[0], START_LOCATION_PARAM) == 0)
			{
				StartLocation = Location(atoi(token[1]), atoi(token[2]), atoi(token[3]));
			}
			else if (strcmp(token[0], GOAL_LOCATION_PARAM) == 0)
			{
				EndPoint = Point(atoi(token[1]), atoi(token[2]));
			}
			else if (strcmp(token[0], ROBOT_SIZE_PARAM) == 0)
			{
				o_robotSize = atoi(token[1]);
			}
			else if (strcmp(token[0], MAP_RESO_PARAM) == 0)
			{
				o_mapResolution = atof(token[1]);
			}
			else if (strcmp(token[0], GRID_RESO_PARAM) == 0)
			{
				o_gridResolution = atof(token[1]);
			}
			else
			{
				printf("unknown param: %s\n", token[0]);
			}
		}
	}

	return true;
}
