/*
 * Map.cpp
 *
 *  Created on: Mar 22, 2016
 *      Author: Or Gafni and Daniel Saad
 */

#include "Map.h"
#include "lodepng.h"
#include <stdio.h>
#include <stdlib.h>

#define CLEAR_CELL_CHAR 	(" ")
#define OBSTACLE_CELL_CHAR 	("*")
#define START_CELL_CHAR 	("S")
#define ROUTE_CELL_CHAR 	("R")
#define FINISH_CELL_CHAR 	("F")
const char* CELL_SYMBOLS[eCellState_numOfStates] = {CLEAR_CELL_CHAR,
												   OBSTACLE_CELL_CHAR,
												   START_CELL_CHAR,
												   ROUTE_CELL_CHAR,
												   FINISH_CELL_CHAR};

const int CLEAR_PIXEL_COLOR[4] = {255,255,255,255};
const int OBSTACLE_PIXEL_COLOR[4] = {0,0,0,255};
const int START_PIXEL_COLOR[4] = {0,255,0,255};
const int ROUTE_PIXEL_COLOR[4] = {255,0,0,255};
const int FINISH_PIXEL_COLOR[4] = {0,0,255,255};

const int* PIXEL_COLOR[eCellState_numOfStates] = {CLEAR_PIXEL_COLOR,
												  OBSTACLE_PIXEL_COLOR,
												  START_PIXEL_COLOR,
												  ROUTE_PIXEL_COLOR,
												  FINISH_PIXEL_COLOR};

Map::Map(double mapResolution, double robotSize) : m_width(0),
												   m_height(0),
												   m_mapResolution(mapResolution),
												   m_robotSize(robotSize)
{
}

Map::~Map()
{
}

bool Map::isCellClear(unsigned char cell)
{
	return cell == 255;
}

void Map::printMap()
{
	for (dword row = 0; row < m_height; row++)
	{
		for (dword col = 0; col < m_width; col++)
		{
			printf("%s",CELL_SYMBOLS[m_map[row][col]]);
		}
		printf("\n");
	}
}

void Map::LoadMap(const char* mapFileName)
{
	unsigned error = lodepng::decode(m_pixels, m_width, m_height, mapFileName);
	if (error)
	{
		printf("decoder error %d : %s\n", error, lodepng_error_text(error));
		exit(-1);
	}
	setSize(m_height, m_width);

	int pixelIter = 0;
	for (dword row = 0; row < m_height; row++)
	{
		for (dword col = 0; col < m_width; col++)
		{
			m_map[row][col] = isCellClear(m_pixels[pixelIter]) ? eCellState_clear : eCellState_obstacle;

			pixelIter+= 4;
		}
	}

//	printMap();
}

void Map::InflateObstacles()
{
	int robotSizeInPixels = m_robotSize / m_mapResolution;
	int inflationRadius = 0.3 * robotSizeInPixels;

	Map tempMap(m_mapResolution, m_robotSize);
	tempMap.setSize(m_height, m_width);

	for (dword row = 0; row < m_height; row++)
	{
		for (dword col = 0; col < m_width; col++)
		{
			if (m_map[row][col] == eCellState_obstacle)
			{
				setObstacle(&tempMap, row, col, inflationRadius);
			}

		}
	}

	m_map = tempMap.m_map;

}

void Map::setObstacle(Map* map, int row, int col, int inflationRadius)
{
	map->m_map[row][col] = eCellState_obstacle;
	for (int i = row - inflationRadius; i <= row + inflationRadius; i++)
	{
		for (int j = col - inflationRadius; j <= col + inflationRadius; j++)
		{
			if (isInMapBoundaries(i, j) == true)
			{
				map->m_map[i][j] = eCellState_obstacle;
			}
		}
	}

}

void Map::SaveMap(const char* mapFileName)
{
	vector<unsigned char> pixels_new;
	pixels_new.resize(m_height*m_width*4);

	int pixelsIter = 0;
	for (dword i = 0; i < m_height; ++i)
	{
		for (dword j = 0; j < m_width; ++j)
		{
			colorCell(pixels_new, i, j, &pixelsIter);
		}

	}

	unsigned error = lodepng::encode(mapFileName, pixels_new, m_width, m_height);
	if (error)
	{
		printf("encoder error %d : %s\n", error, lodepng_error_text(error));
		exit(-1);
	}
}

void Map::setSize(unsigned int height, unsigned int width)
{
	m_height = height;
	m_width = width;

	m_map.resize(m_height);
	for (dword i = 0; i<m_height; i++)
	{
		m_map[i].resize(m_width, eCellState_clear);
	}
}

bool Map::isInMapBoundaries(int i, int j)
{
	return (i >= 0 && i < (int)m_height && j >= 0 && j < (int)m_width);
}

dword Map::GetHeight()
{
	return m_height;
}

dword Map::GetWidth()
{
	return m_width;
}

bool Map::IsCellClear(dword row, dword col)
{
	if (row > m_height || col > m_width)
	{
		printf("row or call are out of bounds\n");
		printf("row = %d, col = %d, m_height = %d, m_width = %d\n", row, col, m_height, m_width);
		exit(0);
	}
	return m_map[row][col] == eCellState_clear;
}

void Map::SetRouteToMap(vector<vector<int> >& intMap)
{
	for (dword row = 0; row < m_height; ++row)
	{
		for (dword col = 0; col < m_width; ++col)
		{
			m_map[row][col] = ECellState(intMap[col][row]);
		}
	}
}

void Map::colorCell(vector<unsigned char>& pixels_new, int row, int col, int* pixelsIter)
{

	pixels_new[(*pixelsIter)++] = PIXEL_COLOR[m_map[row][col]][0];
	pixels_new[(*pixelsIter)++] = PIXEL_COLOR[m_map[row][col]][1];
	pixels_new[(*pixelsIter)++] = PIXEL_COLOR[m_map[row][col]][2];
	pixels_new[(*pixelsIter)++] = PIXEL_COLOR[m_map[row][col]][3];
}
