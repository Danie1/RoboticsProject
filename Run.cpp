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
#include "ConfigurationManager.h"
#include "FilesManager.h"
#include "RouteMngr.h"

void PrintEnlargedGraph(const char* szFilename, Graph& graph)
{
	Map EnlargedMap(ConfigurationManager::Configuration().GetMapResolution(),
			        ConfigurationManager::Configuration().GetRobotSize());

	graph.ConvertToMap(EnlargedMap);

	EnlargedMap.SaveToFile(szFilename);
}

int main()
{
	deque<int> path;
	deque<Point> PointRoute;

	/**
	 * Load the map and inflate it.
	 */
	Map myMap(ConfigurationManager::Configuration().GetMapResolution(),
			  ConfigurationManager::Configuration().GetRobotSize());

	myMap.LoadFromFile(ConfigurationManager::Configuration().GetInputFilename());
	myMap.InflateObstacles_Map();

	/**
	 * Create the corresponding graph
	 */
	Graph graph = Graph(myMap, ConfigurationManager::Configuration().GetGridResolution() /
							   ConfigurationManager::Configuration().GetMapResolution());

	Point StartPointOnGraph = graph.MapToGraphPoint(ConfigurationManager::Configuration().GetStartLocation());
	Point FinishPointOnGraph = graph.MapToGraphPoint(ConfigurationManager::Configuration().GetEndPoint());

	/**
	 * Get the route
	 */
	RouteMngr route_manager(graph, StartPointOnGraph, FinishPointOnGraph);
	deque<Point> newPointRoute = route_manager.GetRoute();

	graph.SaveToFile(FilesManager::Get().GetGraphRouteFile().c_str());

	PrintEnlargedGraph(FilesManager::Get().GetMapRouteFile().c_str() ,graph);

	//Robot robot("10.10.245.63", 6665, newPointRoute[newPointRoute.size() - 1]);
	Robot robot("localhost", 6665, newPointRoute[newPointRoute.size() - 1]);

	robot.SetOdometry(newPointRoute[newPointRoute.size() - 1].GetX(),
					  newPointRoute[newPointRoute.size() - 1].GetY(),
					  ConfigurationManager::Configuration().GetStartLocation().GetYaw());
	robot.Read();

	Location currLoc = Location(newPointRoute[newPointRoute.size() - 1].GetX(),
							    newPointRoute[newPointRoute.size() - 1].GetY(),
							    ConfigurationManager::Configuration().GetStartLocation().GetYaw());

	 // Create the first particles
	Localization localization(graph);

	Driver driver(robot, localization);

    Particle* p = new Particle(robot.GetX(), robot.GetY(), robot.GetYaw(), 0.51);

	localization.particles.push_back(p);

	for (int i = 0; i<MAX_PARTICLES_COUNT; i++)
	{
		localization.particles.push_back(p->CreateChild(5, 1, graph));
	}

	localization.PrintParticlesOnMap(FilesManager::Get().GetNextParticleFile().c_str());

	int i = 0;

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

		localization.PrintParticlesOnMap(FilesManager::Get().GetNextParticleFile().c_str());
	}

	printf("FINISH!!!\n");

	return 0;
}


