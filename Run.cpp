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



#define MAX_FILE_PATH (30)
const char* PARAMS_FILE_NAME = "parameters.txt";


void printMap(vector<vector <int> >& intMap, int xSize, int ySize)
{
    // display the map with the route
    for(int y=0;y<ySize;y++)
    {
        for(int x=0;x<xSize;x++)
            if((intMap)[x][y]==0)
                cout<<".";
            else if((intMap)[x][y]==1)
                cout<<"O"; //obstacle
            else if((intMap)[x][y]==2)
                cout<<"S"; //start
            else if((intMap)[x][y]==3)
                cout<<"R"; //route
            else if((intMap)[x][y]==4)
                cout<<"F"; //finish
        cout<<endl;
    }
}

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

void displayRoute(deque<int> route, Graph* myMap, int xStart, int yStart)
{
	vector<vector <int> > map;

	createIntMap(map, myMap);


//	int ySize = myMap->GetHeight();
//	int xSize = myMap->GetWidth();
//	printMap(map, xSize, ySize);

// follow the route on the map and display it
    if(route.size()>0)
    {
        int j; char c;
        int x=xStart;
        int y=yStart;
        map[x][y]=2;
        for(int i=route.size() - 1;i>=0;i--)
        {
        	j = route.at(i);

            if (j < 0 || j > 3)
            {
            	printf("Error in displaying route! - Got %d \r\n", j);
            	continue;
            }

            x=x+dx[j];
            y=y+dy[j];
            map[x][y]=3;
        }
        map[x][y]=4;

//        printMap(map, xSize, ySize);

    }

    myMap->SetRouteOnGraph(map);
}

int main()
{
	const dword MAP_TO_GRAPH_RATIO = 4;

	char inputMapFileName[MAX_FILE_PATH];
	char outputMapFileName[MAX_FILE_PATH];
	double mapResolution;
	double robotSize;
	deque<int> path;
	dword xStart, xFinish, yStart, yFinish = 0;

	if (Utils::getParamsFromFile(PARAMS_FILE_NAME, inputMapFileName, outputMapFileName, &mapResolution, &robotSize) == false)
	{
		printf("failed to get params\n");
		exit(-1);
	}
	Map myMap(mapResolution, robotSize);

	myMap.LoadFromFile(inputMapFileName);
	myMap.InflateObstacles_Map();

	Graph graph = Graph(myMap, MAP_TO_GRAPH_RATIO);

	do {
		bool fSuccess = graph.GetRandomStartAndFinishPoints(xStart, yStart, xFinish, yFinish);

		AStar pathSolver(&graph);
		path = pathSolver.pathFind(xStart, yStart, xFinish, yFinish);
	} while (path.empty());

	//cout << path << endl;
	displayRoute(path, &graph, xStart, yStart);


	graph.SaveToFile(outputMapFileName);

	Map newMap(mapResolution, robotSize);

	graph.ConvertToMap(newMap);

	newMap.SaveToFile("CoolMap.png");

	printf("FINISH!!!\n");

	return 0;
}


