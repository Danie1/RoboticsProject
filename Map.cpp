/*
 * Map.cpp
 *
 *  Created on: Mar 22, 2016
 *      Author: Or Gafni and Daniel Saad
 */

#include "Map.h"
#include <cstdlib>
#include <ctime>
#include <utility>

Map::Map(double mapResolution, double robotSize) : CellMatrix(),
												   m_mapResolution(mapResolution),
												   m_robotSize(robotSize)
{
}

Map::Map(Map &map) : m_mapResolution(map.GetResolution()),
					 m_robotSize(map.GetRobotSize())
{
	Resize(map.GetHeight(), map.GetWidth());
	CopyMatrix(map);
}

Map::~Map()
{
}

void Map::InflateObstacles_Map()
{
	int robotSizeInPixels = m_robotSize / m_mapResolution;
	int inflationRadius = 0.3 * robotSizeInPixels;

	Map tempMap(m_mapResolution, m_robotSize);
	tempMap.Resize(GetHeight(), GetWidth());

	for (dword row = 0; row < GetHeight(); row++)
	{
		for (dword col = 0; col < GetWidth(); col++)
		{
			if (GetCell(row, col) == eCellState_obstacle)
			{
				InflateObstacles_Cell(tempMap, row, col, inflationRadius);
			}

		}
	}

	CopyMatrix(tempMap);
}

void Map::InflateObstacles_Cell(Map& map, int row, int col, int inflationRadius)
{
	// Set the current cell as an obstacle
	map.SetCell(row, col, eCellState_obstacle);

	// Set the surrounding cells as obstacles.
	for (int i = row - inflationRadius; i <= row + inflationRadius; i++)
	{
		for (int j = col - inflationRadius; j <= col + inflationRadius; j++)
		{
			if (isInMapBoundaries(i, j) == true)
			{
				map.SetCell(i, j, eCellState_obstacle);
			}
		}
	}

}

bool Map::isInMapBoundaries(int i, int j)
{
	return (i >= 0 && i < (int)GetHeight() && j >= 0 && j < (int)GetWidth());
}