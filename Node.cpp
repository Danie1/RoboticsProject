/*
 * Node.cpp
 *
 *  Created on: Mar 29, 2016
 *      Author: Or Gafni
 */

#include "Node.h"

Node::Node(dword row, dword col)
{
	m_neighbors.resize(DIRECTIONS_NUM);

	for (int i = 0; i < DIRECTIONS_NUM; ++i)
	{
		m_neighbors[i] = NULL;
	}
}

Node::~Node() {
	// TODO Auto-generated destructor stub
}

void Node::SetNeighbor(Direction neighborDir, Node* neighbor)
{
	m_neighbors[neighborDir] = neighbor;
}

