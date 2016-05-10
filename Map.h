/*
 * Map.h
 *
 *  Created on: Mar 22, 2016
 *      Author: Or Gafni and Daniel Saad
 */

#ifndef MAP_H_
#define MAP_H_


#include <vector>
using namespace std;

#include "typedefs.h"

class Map
{
	private:
		dword m_width;
		dword m_height;
		vector<vector<bool> > m_map;
		vector<byte> m_pixels;
		double m_mapResolution; // pixels in map per meter
		double m_robotSize;


		void printMap();
		bool IsCellClear(byte cell);
		void setObstacle(Map* map, int row, int col, int inflationRadius);

	public:
		Map(double mapResolution, double robotSize);
		virtual ~Map();

		void LoadMap(const char* mapFileName);
		void InflateObstacles();
		void SaveMap(const char* mapFileName);

	private:

		void setSize(unsigned int height, unsigned int width);
		bool isInMapBoundaries(int i, int j);
};

#endif /* MAP_H_ */
