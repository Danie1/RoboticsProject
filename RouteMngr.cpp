#include "RouteMngr.h"

#include "AStar.h"
using namespace std;

RouteMngr::RouteMngr(Graph& graph, Point StartPoint, Point FinishPoint) : m_graph(graph),
																		  m_StartPoint(StartPoint),
																		  m_FinishPoint(FinishPoint)
{
	deque<Point> PointRoute;
	deque<int> path;

	AStar pathSolver(&graph);
	path = pathSolver.pathFind(StartPoint.GetX(), StartPoint.GetY(),
							   FinishPoint.GetX(), FinishPoint.GetY());

	m_PointRoute = GetRoute(path);

	printf("Initial Waypoints list:\n----------------------\r\n");
	PrintRoute();

	m_PointRoute = ImprovePointRoute();

	printf("Improved Waypoints list:\n----------------------\r\n");
	PrintRoute();
}

std::deque<Point> RouteMngr::GetRoute()
{
	return m_PointRoute;
}

deque<Point> RouteMngr::ReverseRoute(deque<Point> route)
{
	deque<Point> reverseRoute;
	for (int i = route.size() - 1; i >= 0; i--)
	{
		reverseRoute.push_back(route[i]);
	}

	return reverseRoute;
}

void RouteMngr::PrintRoute()
{
	printf("Route Begin; \r\n");

	deque<Point> RouteToPrint = ReverseRoute(m_PointRoute);

	for (dword i = 0; i< RouteToPrint.size(); i++)
	{
		printf("%d. (%f, %f) \r\n", i + 1, RouteToPrint[i].GetX(), RouteToPrint[i].GetY());
	}
	printf("Route Ended; \r\n");
}

deque<Point> RouteMngr::ImprovePointRoute()
{
	deque<Point> newRoute;

	deque<Point> reverseRoute;

	reverseRoute = ReverseRoute(m_PointRoute);

	// Put the first waypoint in the new route
	newRoute.push_back(reverseRoute[0]);
	int lastWaypointIndex = 0;

	for(int i = 2; i < reverseRoute.size(); i++)
	{
		if (m_graph.IsThereObstacleBetweenPoints(reverseRoute[lastWaypointIndex], reverseRoute[i]))
		{
			newRoute.push_back(reverseRoute[i - 1]);
			lastWaypointIndex = i - 1;
		}
	}

	if (lastWaypointIndex != reverseRoute.size() - 1)
	{
		newRoute.push_back(reverseRoute[reverseRoute.size() - 1]);
	}

	for (int i = 0; i < newRoute.size(); ++i)
	{
		m_graph.SetCell(newRoute[i].GetY(), newRoute[i].GetX(), ecellState_waypoint);
	}

	return ReverseRoute(newRoute);
}

deque<Point> RouteMngr::GetRoute(deque<int> route)
{
	const int MAX_POINTS_TO_IGNORE = 1000;

	deque<Point> PointRoute;

	// follow the route on the map and display it
    if(route.size()>0)
    {
        int j;
        int x = m_StartPoint.GetX();
        int y = m_StartPoint.GetY();
        m_graph.SetCell(y,x, eCellState_start);
        int lastDirection = 1000;
        int pointsIgnoredCounter = 0;

        for(int i=route.size() - 1;i>=0;i--)
        {
        	j = route.at(i);

            if (j < 0 || j >= dir)
            {
            	printf("Error in displaying route! - Got %d \r\n", j);
            	continue;
            }

            // If we are in the same direction, no need to create new point
            if (j == lastDirection && pointsIgnoredCounter < MAX_POINTS_TO_IGNORE)
            {
            	pointsIgnoredCounter++;
            }
            else
            {
            	PointRoute.push_front(Point(x,y));
            	lastDirection = j;
            	pointsIgnoredCounter = 0;
            }

            x=x+dx[j];
            y=y+dy[j];

            m_graph.SetCell(y,x, eCellState_route);
        }

        m_graph.SetCell(y,x, eCellState_finish);

        PointRoute.push_front(Point(x,y));
    }

    return PointRoute;
}

RouteMngr::~RouteMngr()
{

}
