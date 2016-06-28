/*
 * Run.cpp
 *
 *  Created on: Mar 8, 2016
 *      Author: Or Gafni and Daniel Saad
 */
#include "Map.h"
#include "Graph.h"
#include <stdlib.h>
#include "stdio.h"
#include "Utils.h"
#include "AStar.h"
#include <string>
#include <deque>
#include <iostream>
#include "Robot.h"
#include "Driver.h"
#include "CommonStructs.h"
#include "Math.h"
#include "Definitions.h"
#include "LocalizationManager.h"
#include "Particle.h"

#include "RouteMngr.h"

#define MAX_FILE_PATH (30)
const char* PARAMS_FILE_NAME = "parameters.txt";

int main()
{
	const dword MAP_TO_GRAPH_RATIO = 4;

	char inputMapFileName[MAX_FILE_PATH];
	double mapResolution;
	double gridResolution;
	int robotSize;

	deque<int> path;
	deque<Point> PointRoute;

	Location StartLocation(0,0,0);
	Point    EndPoint(0,0);

	/**
	 * Get the configuration from the file.
	 */
	if (Utils::getParamsFromFile(PARAMS_FILE_NAME, // Parameters.txt
								inputMapFileName, // Where the map file image is
								StartLocation, // Where the robot starts
								EndPoint, // Where the robot finishes
								robotSize, // The robot size. We assume its a square atm
								mapResolution, // The map resolution
								gridResolution) == false) // The grid resolution
	{
		printf("failed to get params\n");
		exit(-1);
	}

	/**
	 * Load the map and inflate it.
	 */
	Map myMap(mapResolution, robotSize);

	myMap.LoadFromFile(inputMapFileName);
	myMap.InflateObstacles_Map();

	/**
	 * Create the corresponding graph
	 */
	Graph graph = Graph(myMap, MAP_TO_GRAPH_RATIO);

	Point StartPointOnGraph = graph.MapToGraphPoint(StartLocation);
	Point FinishPointOnGraph = graph.MapToGraphPoint(EndPoint);

	/**
	 * Get the route
	 */
	RouteMngr route_manager(graph, StartPointOnGraph, FinishPointOnGraph);
	deque<Point> newPointRoute = route_manager.GetRoute();

	graph.SaveToFile("RouteGraph.png");

	Map EnlargedMap(mapResolution, robotSize);

	graph.ConvertToMap(EnlargedMap);

	EnlargedMap.SaveToFile("EnlargedMap.png");

	Robot robot("10.10.245.63", 6665, robotSize / mapResolution, newPointRoute[newPointRoute.size() - 1]);
//	Robot robot("localhost", 6665, robotSize / mapResolution, newPointRoute[newPointRoute.size() - 1]);

	printf("setOdo to (%f, %f, %d)\r\n", newPointRoute[newPointRoute.size() - 1].GetX(),
			  newPointRoute[newPointRoute.size() - 1].GetY(),
			  StartLocation.GetYaw());
	robot.SetOdometry(newPointRoute[newPointRoute.size() - 1].GetX(),
					  newPointRoute[newPointRoute.size() - 1].GetY(),
					  StartLocation.GetYaw());
	robot.Read();
	printf("getOdo (%f, %f, %f)\r\n", robot.GetRobotX(), robot.GetRobotY(), robot.GetYaw());

	Location currLoc = Location(newPointRoute[newPointRoute.size() - 1].GetX(),
							    newPointRoute[newPointRoute.size() - 1].GetY(),
							    StartLocation.GetYaw());


	Graph partGraph = graph;
	 // Create the first particles
	Localization localization(graph);

	Driver driver(robot, localization);

    Particle* p = new Particle(robot.GetX(), robot.GetY(), robot.GetYaw(), 0.51);

	localization.particles.push_back(p);

	for (int i = 0; i<MAX_PARTICLES_COUNT; i++)
	{
		localization.particles.push_back(p->CreateChild(5, 1, partGraph));
	}

	for (int i = 0; i < localization.particles.size(); ++i)
	{
		partGraph.SetCell(localization.particles[i]->GetY(), localization.particles[i]->GetX(), ecellState_particle);
	}

	Map EnlargedMap1(mapResolution, robotSize);

	partGraph.ConvertToMap(EnlargedMap1);

	EnlargedMap1.SaveToFile("initParticle1.png");
	robot.SetSpeed(0,1);

	int i = 0;
	printf("FinishPointOnGraph (%f,%f)\n", FinishPointOnGraph.GetX(), FinishPointOnGraph.GetY());
	while (!robot.InRadius(newPointRoute[0]))
	{

		i++;
		if (i > newPointRoute.size())
		{
			printf("we go through all waypoints and still the robot didn't reach his destination :(\n");
			break;
		}
		driver.MoveToWayPoint(newPointRoute[newPointRoute.size() - i]);
		cout << "Moved to next waypoint!. press enter to continue" << endl;
	}

	printf("FINISH!!!\n");

	return 0;
}


