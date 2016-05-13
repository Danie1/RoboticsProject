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
#include "AStar.h"
#include <string>
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

void createIntMap(vector<vector <int> >& intMap, Map* myMap)
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

void displayRoute(string route, Map* myMap, int xStart, int yStart)
{
	vector<vector <int> > map;

	createIntMap(map, myMap);

	int ySize = myMap->GetHeight();
	int xSize = myMap->GetWidth();

	printMap(map, xSize, ySize);

// follow the route on the map and display it
    if(route.length()>0)
    {
        int j; char c;
        int x=xStart;
        int y=yStart;
        map[x][y]=2;
        for(int i=0;i<route.length();i++)
        {
            c =route.at(i);
            j=atoi(&c);
            x=x+dx[j];
            y=y+dy[j];
            map[x][y]=3;
        }
        map[x][y]=4;

        printMap(map, xSize, ySize);

    }
}

int main()
{
	char inputMapFileName[MAX_FILE_PATH];
	char outputMapFileName[MAX_FILE_PATH];
	double mapResolution;
	double robotSize;
	int xStart = 104;
	int xFinish = 270;
	int yStart = 109;
	int yFinish = 270;
	if (Utils::getParamsFromFile(PARAMS_FILE_NAME, inputMapFileName, outputMapFileName, &mapResolution, &robotSize) == false)
	{
		printf("failed to get params\n");
		exit(-1);
	}

	Map myMap(mapResolution, robotSize);
	myMap.LoadMap(inputMapFileName);
	myMap.InflateObstacles();
//	myMap.printMap();

	AStar pathSolver(&myMap);
	string path = pathSolver.pathFind(xStart, yStart, xFinish, yFinish);

	std:cout << path << endl;

	displayRoute(path, &myMap, xStart, yStart);

	// Create Graph from map
//	Graph myGraph;
//	myGraph.BuildGraphFromMap(myMap);

	myMap.SaveMap(outputMapFileName);

	printf("FINSIH!!!\n");

	return 0;
}


