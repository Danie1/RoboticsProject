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

#define MAX_FILE_PATH (30)
const char* PARAMS_FILE_NAME = "parameters.txt";

void createIntMap(vector<vector <int> >& intMap, CellMatrix* myMap)
{
	int xSize = myMap->GetWidth();
	int ySize = myMap->GetHeight();

	intMap.resize(xSize);
	for (int i = 0; i < xSize;i++)
	{
		intMap[i].resize(ySize);
	}

	for (int x = 0; x < xSize;x++)
	{
//		printf(" i = %d\n",i);
		for (int y = 0; y < ySize;y++)
		{
//			printf(" j = %d\n",j);
			if (myMap->IsCellClear(y, x) == true)
			{
				(intMap)[x][y] = 0;
			}
			else
			{
				(intMap)[x][y] = 1;
			}
		}
	}
}

deque<Point> myFunc(deque<Point> route)
{
	deque<Point> reverseRoute;
	for (int i = route.size() - 1; i >= 0; i--)
	{
		reverseRoute.push_back(route[i]);
	}

	return reverseRoute;
}

void printRoute(deque<Point> route)
{
	cout << "Route Begin;" << endl;

	for (dword i = 0; i< route.size(); i++)
	{
		cout << "(" << route[i].GetX() << "," << route[i].GetY() << ")" << endl;
	}
	cout << "Route Ended;" << endl;
}

deque<Point> improvePointRoute(deque<Point> originalRoute, Graph* myMap)
{
	deque<Point> newRoute;

//	printf("originalRoute: \n");
//	printRoute(originalRoute);
	deque<Point> reverseRoute;

	reverseRoute = myFunc(originalRoute);

//	printf("reverseRoute: \n");
//	printRoute(reverseRoute);
//	reverseRoute = reverseRoute(originalRoute);

	// Put the first waypoint in the new route
	newRoute.push_back(reverseRoute[0]);
	int lastWaypointIndex = 0;

	for(int i = 2; i < reverseRoute.size(); i++)
	{
//		printf("checking line between waypoint1 (%f, %f) and waypoint (%f, %f)\n",
//			reverseRoute[lastWaypointIndex].GetX(), reverseRoute[lastWaypointIndex].GetY(), reverseRoute[i].GetX(), reverseRoute[i].GetY());
		if (myMap->IsThereObstacleBetweenPoints(reverseRoute[lastWaypointIndex], reverseRoute[i]))
		{
			newRoute.push_back(reverseRoute[i - 1]);
			lastWaypointIndex = i - 1;
		}
//		myMap->printMatrix();
	}

	if (lastWaypointIndex != reverseRoute.size() - 1)
	{
		newRoute.push_back(reverseRoute[reverseRoute.size() - 1]);
	}

	printf("newRoute:\n");
	printRoute(newRoute);

	for (int i = 0; i < newRoute.size(); ++i)
	{
		myMap->SetCell(newRoute[i].GetY(), newRoute[i].GetX(), ecellState_waypoint);
	}

	return myFunc(newRoute);
//	return newRoute;
}

deque<Point> displayRoute(deque<int> route, Graph* myMap, int xStart, int yStart)
{

	const int MAX_POINTS_TO_IGNORE = 1000;
	vector<vector <int> > map;

	deque<Point> PointRoute;

	createIntMap(map, myMap);

// follow the route on the map and display it
    if(route.size()>0)
    {
        int j;
        int x=xStart;
        int y=yStart;
        map[x][y]=2;
        int lastDirection = 1000;
        int pointsIgnoredCounter = 0;

        for(int i=route.size() - 1;i>=0;i--)
        {
        	j = route.at(i);

            if (j < 0 || j >= dir)
            {
            	printf("Error in displaying route! - Got %d \r\n", j);
            	continue;
            }

            // If we are in the same direction, no need to create new point
            if (j == lastDirection && pointsIgnoredCounter < MAX_POINTS_TO_IGNORE)
            {
            	pointsIgnoredCounter++;
            }
            else
            {
            	PointRoute.push_front(Point(x,y));
            	lastDirection = j;
            	pointsIgnoredCounter = 0;
            }

            x=x+dx[j];
            y=y+dy[j];
//            PointRoute.push_front(Point(x,y));
            map[x][y]=3;


        }
        map[x][y]=4;
        PointRoute.push_front(Point(x,y));

    }

    myMap->SetRouteOnGraph(map);

    return PointRoute;
}

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
	Map myMap(mapResolution, robotSize);

	myMap.LoadFromFile(inputMapFileName);
	myMap.InflateObstacles_Map();

	Graph graph = Graph(myMap, MAP_TO_GRAPH_RATIO);

	Point StartPointOnGraph = graph.MapToGraphPoint(StartLocation);
	Point FinishPointOnGraph = graph.MapToGraphPoint(EndPoint);

	do {
		//bool fSuccess = graph.GetRandomStartAndFinishPoints(xStart, yStart, xFinish, yFinish);



		AStar pathSolver(&graph);
		path = pathSolver.pathFind(StartPointOnGraph.GetX(), StartPointOnGraph.GetY(),
								   FinishPointOnGraph.GetX(), FinishPointOnGraph.GetY());

		if (!path.empty()){
			cout << "Found a path to destination." << endl;
		}
		else
		{
			cout << "No path to destination." << endl;
			return -1;
		}

	} while (false);//path.empty());


	PointRoute = displayRoute(path, &graph, StartPointOnGraph.GetX(), StartPointOnGraph.GetY());

	cout << "Route Begin;" << endl;

	for (dword i = 0; i< PointRoute.size(); i++)
	{
		cout << "(" << PointRoute[i].GetX() << "," << PointRoute[i].GetY() << ")" << endl;
	}
	cout << "Route Ended;" << endl;

	deque<Point> newPointRoute = improvePointRoute(PointRoute, &graph);

#if 1
	graph.SaveToFile("RouteGraph.png");

	Map EnlargedMap(mapResolution, robotSize);

	graph.ConvertToMap(EnlargedMap);

	EnlargedMap.SaveToFile("EnlargedMap.png");

	Robot robot("localhost", 6665, robotSize / mapResolution, newPointRoute[newPointRoute.size() - 1]);

	robot.SetOdometry(newPointRoute[newPointRoute.size() - 1].GetX(), newPointRoute[newPointRoute.size() - 1].GetY(), StartLocation.GetYaw());
	robot.Read();

	Driver driver(robot);

	//driver.TurnToPoint(PointRoute[PointRoute.size() - 2]);

	//driver.MoveToWayPoint(PointRoute[PointRoute.size() - 7]);

	Location currLoc = Location(newPointRoute[newPointRoute.size() - 1].GetX(), newPointRoute[newPointRoute.size() - 1].GetY(), 20);

	double angle = 0;

	for (int i = 2; i <= newPointRoute.size(); i++)
	{
		//driver.MoveToWayPoint(PointRoute[PointRoute.size() - i]);
		//driver.moveToNextWaypoint(Point::GetPixelPointInCM(PointRoute[i]));
		angle = currLoc.GetAngleFrom(newPointRoute[newPointRoute.size() - i]);
		printf("The new angle is: %f \r\n", angle);
		currLoc = newPointRoute[newPointRoute.size() - i];
	}


#if 1
	char temp[100];
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
		//driver.moveToNextWaypoint(Point::GetPixelPointInCM(PointRoute[i]));
		cout << "Moved to next waypoint!. press enter to continue" << endl;
//		scanf("%s", temp);
	}

	//driver.moveToNextWaypoint(3, -3);

#endif
#endif
	printf("FINISH!!!\n");

	return 0;
}


