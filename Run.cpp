/*
 * Run.cpp
 *
 *  Created on: Mar 8, 2016
 *      Author: Or Gafni and Daniel Saad
 */
#include "Map.h"
#include <stdlib.h>
#include "stdio.h"
#include "Utils.h"
#include "Graph.h"


#define MAX_FILE_PATH (30)
const char* PARAMS_FILE_NAME = "parameters.txt";

int main()
{
	char inputMapFileName[MAX_FILE_PATH];
	char outputMapFileName[MAX_FILE_PATH];
	double mapResolution;
	double robotSize;
	if (Utils::getParamsFromFile(PARAMS_FILE_NAME, inputMapFileName, outputMapFileName, &mapResolution, &robotSize) == false)
	{
		printf("failed to get params\n");
		exit(-1);
	}

	Map myMap(mapResolution, robotSize);
	myMap.LoadMap(inputMapFileName);
	myMap.InflateObstacles();

	// Create Graph from map
	Graph myGraph;
	myGraph.BuildGraphFromMap(myMap);

	myMap.SaveMap(outputMapFileName);

	printf("FINSIH!!!\n");

	return 0;
}


