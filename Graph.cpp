/*
 * Graph.cpp
 *
 *  Created on: Mar 29, 2016
 *      Author: Or Gafni
 */

#include "Graph.h"

Graph::Graph() {
	// TODO Auto-generated constructor stub

}

Graph::~Graph() {
	// TODO Auto-generated destructor stub
}

void Graph::BuildGraphFromMap(Map &map)
{
	m_nodes.resize(map.GetHeight());

	for (int rowIndex = 0; rowIndex < map.GetHeight(); ++rowIndex)
	{
		m_nodes[rowIndex].resize(map.GetWidth());
	}
	height = map.GetHeight();
	width = map.GetWidth();


	// Setting a node in the graph for each cell in the map
	for (int rowIndex = 0; rowIndex < map.GetHeight(); ++rowIndex)
	{
		for (int colIndex = 0; colIndex < map.GetWidth(); ++colIndex)
		{
			if (map.IsCellClear(rowIndex, colIndex))
			{
				m_nodes[rowIndex][colIndex] = new Node(rowIndex, colIndex);
			}
			else
			{
				m_nodes[rowIndex][colIndex] = NULL;
			}
		}
	}

	// Scan all the nodes in the graph and set neighboors
	for (int rowIndex = 0; rowIndex < map.GetHeight(); ++rowIndex)
	{
		for (int colIndex = 0; colIndex < map.GetWidth(); ++colIndex)
		{
			// There is node in the graph
			if (m_nodes[rowIndex][colIndex] != NULL)
			{
				setAllNeigbors(rowIndex, colIndex);
			}
		}
	}

	printGraph();

}

void Graph::setAllNeigbors(int rowIndex, int colIndex)
{
	if (colIndex > 0)
	{
		m_nodes[rowIndex][colIndex]->SetNeighbor(LEFT, m_nodes[rowIndex][colIndex - 1]);
	}
	if (colIndex < m_nodes[0].size() - 1)
	{
		m_nodes[rowIndex][colIndex]->SetNeighbor(RIGHT, m_nodes[rowIndex][colIndex + 1]);
	}
	if (rowIndex > 0)
	{
		m_nodes[rowIndex][colIndex]->SetNeighbor(UP, m_nodes[rowIndex - 1][colIndex]);
	}
	if (rowIndex < m_nodes.size() - 1)
	{
		m_nodes[rowIndex][colIndex]->SetNeighbor(DOWN, m_nodes[rowIndex + 1][colIndex]);
	}

}

void Graph::printGraph()
{
//	for (int rowIndex = 0; rowIndex < height; ++rowIndex)
//	{
//		for (int colIndex = 0; colIndex < width; ++colIndex)
//		{
//			if (m_nodes[rowIndex, colIndex] != NULL)
//			{
//				printf("*")
//				m_nodes[rowIndex][colIndex] = new Node(rowIndex, colIndex);
//			}
//			else
//			{
//				m_nodes[rowIndex][colIndex] = NULL;
//			}
//		}
//	}
}
