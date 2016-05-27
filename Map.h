/*
 * Map.h
 *
 *  Created on: Mar 22, 2016
 *      Author: Or Gafni and Daniel Saad
 */

#ifndef MAP_H_
#define MAP_H_

#include <functional>

#include "CellMatrix.h"

class Map : public CellMatrix
{
	private:
		double m_mapResolution; // pixels in map per meter
		double m_robotSize;


	public:
		Map(double mapResolution, double robotSize);
		Map(Map &map);
		virtual ~Map();

		double GetResolution() {return m_mapResolution;}
		double GetRobotSize() {return m_robotSize;}

		void LoadMap(const char* mapFileName);
		void SaveMap(const char* mapFileName);

		void InflateObstacles_Map();
		void InflateMap(Map& new_map, int ratio);
		
		void SetRouteToMap(vector<vector<int> >& intMap);
		
	private:

		bool IsThereAnObstacle(int beg_row, int beg_col, int end_row, int end_col);
		void InflateObstacles_Cell(Map& map, int row, int col, int inflationRadius);
		bool isInMapBoundaries(int i, int j);
};

#endif /* MAP_H_ */
