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

#define OBSTACLE_CELL_CHAR 	("*")
#define CLEAR_CELL_CHAR 	(" ")

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
			if (m_map[row][col] == false)
			{
				printf(OBSTACLE_CELL_CHAR);
			}
			else
			{
				printf(CLEAR_CELL_CHAR);
			}
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
			m_map[row][col] = isCellClear(m_pixels[pixelIter]);

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
			if (m_map[row][col] == false)
			{
				setObstacle(&tempMap, row, col, inflationRadius);
			}

		}
	}

	m_map = tempMap.m_map;

}

void Map::setObstacle(Map* map, int row, int col, int inflationRadius)
{
	map->m_map[row][col] = false;
	for (int i = row - inflationRadius; i <= row + inflationRadius; i++)
	{
		for (int j = col - inflationRadius; j <= col + inflationRadius; j++)
		{
			if (isInMapBoundaries(i, j) == true)
			{
				map->m_map[i][j] = false;
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
			if (m_map[i][j] == true)
			{
				pixels_new[pixelsIter++] = 255;
				pixels_new[pixelsIter++] = 255;
				pixels_new[pixelsIter++] = 255;
				pixels_new[pixelsIter++] = 255;
			}
			else
			{
				pixels_new[pixelsIter++] = 0;
				pixels_new[pixelsIter++] = 0;
				pixels_new[pixelsIter++] = 0;
				pixels_new[pixelsIter++] = 0;
			}
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
		m_map[i].resize(m_width, true);
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
		exit(0);
	}
	return m_map[row][col];
}
