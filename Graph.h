/*
 * Graph.h
 *
 *  Created on: Mar 29, 2016
 *      Author: Or Gafni
 */

#ifndef GRAPH_H_
#define GRAPH_H_

#include "Node.h"
#include "Map.h"

#include <vector>

using namespace std;

class Graph {
private:
	vector<vector<Node*> > m_nodes;
	int height;
	int width;

public:
	Graph();
	virtual ~Graph();

	void BuildGraphFromMap(Map &map);

private:
	void setAllNeigbors(int rowIndex, int colIndex);
	void printGraph();
};

#endif /* GRAPH_H_ */
