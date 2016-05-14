/*
 * Node.h
 *
 *  Created on: Mar 29, 2016
 *      Author: Or Gafni
 */

#ifndef NODE_H_
#define NODE_H_

#include <vector>
#include "typedefs.h"
using namespace std;

#define DIRECTIONS_NUM (4)

enum Direction {LEFT, RIGHT, UP, DOWN};

class Node
{

private:
	dword m_row;
	dword m_col;
	vector<Node*> m_neighbors;

public:
	Node(dword row, dword col);

	virtual ~Node();

	void SetNeighbor(Direction neighborDir, Node* neighbor);
};

#endif /* NODE_H_ */
