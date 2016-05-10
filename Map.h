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


	public:
		Map(double mapResolution, double robotSize);
		virtual ~Map();

		void LoadMap(const char* mapFileName);
		void InflateObstacles();
		void SaveMap(const char* mapFileName);
		bool IsCellClear(dword row, dword col);

		vector<vector<bool> > GetCells();

		dword GetHeight();
		dword GetWidth();

	private:


		void printMap();
		bool isCellClear(byte cell);
		void setObstacle(Map* map, int row, int col, int inflationRadius);
		void setSize(unsigned int height, unsigned int width);
		bool isInMapBoundaries(int i, int j);
};

#endif /* MAP_H_ */
