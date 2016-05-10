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


#define INPUT_MAP_PARAM ("inMap:")
#define OUTPUT_MAP_PARAM ("outMap:")
#define ROBOT_SIZE_PARAM ("robotSizeM:")
#define MAP_RESO_PARAM ("MapResolutionM:")

bool Utils::getParamsFromFile(const char* paramsFileName,
							  char* o_inputMapFileName,
							  char* o_outputMapFileName,
							  double* o_mapResolution,
							  double* o_robotSize)
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
			else if (strcmp(token[0], OUTPUT_MAP_PARAM) == 0)
			{
				strcpy(o_outputMapFileName, token[1]);
			}
			else if (strcmp(token[0], ROBOT_SIZE_PARAM) == 0)
			{
				*o_robotSize = atof(token[1]);
			}
			else if (strcmp(token[0], MAP_RESO_PARAM) == 0)
			{
				*o_mapResolution = atof(token[1]);
			}
			else
			{
				printf("unknown param: %s\n", token[0]);
			}
		}
	}

	return true;
}
