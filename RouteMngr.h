#pragma once

#include "CommonStructs.h"
#include <deque>
#include "Graph.h"

class RouteMngr
{
public:
	RouteMngr(Graph& graph, Point StartPoint, Point FinishPoint);
	~RouteMngr();
	std::deque<Point> GetRoute();

private:
	std::deque<Point> GetRoute(std::deque<int> route);
	std::deque<Point> ReverseRoute(std::deque<Point> route);
	void PrintRoute();
	std::deque<Point> ImprovePointRoute();



	Graph& m_graph;
	Point m_StartPoint;
	Point m_FinishPoint;

	std::deque<Point> m_PointRoute;
};
