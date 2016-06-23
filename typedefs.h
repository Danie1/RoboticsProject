/*
 * typedefs.h
 *
 *  Created on: Apr 15, 2016
 *      Author: Or Gafni and Daniel Saad
 */

#ifndef TYPEDEFS_H_
#define TYPEDEFS_H_

typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned int dword;
typedef unsigned long long int qword;

enum ECellState
{
	eCellState_clear,
	eCellState_obstacle,
	eCellState_start,
	eCellState_route,
	eCellState_finish,
	ecellState_debug,
	ecellState_waypoint,
	eCellState_numOfStates
};

#endif /* TYPEDEFS_H_ */
