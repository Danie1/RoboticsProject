/*
 * Map.h
 *
 *  Created on: Mar 22, 2016
 *      Author: Or Gafni and Daniel Saad
 */
#ifndef GRAPH_H_
#define GRAPH_H_

#include "CellMatrix.h"
#include "Map.h"

class Graph : public CellMatrix
{
private:
	dword m_ratio;

	void FromMapToGraph(Map& map, dword ratio);

public:
	Graph(Map& map, int Ratio);

	void SetOriginalCell(int row, int col, ECellState eType);
	ECellState GetOriginalCell(int row, int col);
	bool IsOriginalCellClear(dword row, dword col);

	void ConvertToMap(Map& map);

	void SetRouteOnGraph(vector<vector<int> >& intMap);

	int GetGraphRatio() { return m_ratio; }

	bool GetRandomStartAndFinishPoints(dword& xStart, dword& yStart, dword& xFinish, dword& yFinish);

};

#endif