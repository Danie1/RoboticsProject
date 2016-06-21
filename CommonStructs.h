#pragma once

#include <cmath>
#include <math.h>
#include "Math.h"
#include <stdio.h>

const int INVALID_YAW = -1;

struct Point
{
public:
	Point(int X, int Y) { m_X = X; m_Y = Y;}

	int GetX() { return m_X;}
	int GetY() { return m_Y; }
	void SetX(int X) { m_X = X;	}
	void SetY(int Y) { m_Y = Y;	}

	Point& operator=(Point other) // copy assignment
	{
	    if (this != &other) { // self-assignment check expected
	    	m_X = other.GetX();
	    	m_Y = other.GetY();
	    }

	    return *this;
	}

	static Point GetPixelPointInCM(Point pnt)
	{
		return Point(pnt.GetX() / 10, pnt.GetY() / 10);
	}

	static Point GetCMPointInPixel(Point pnt)
	{
		return Point(pnt.GetX() * 10, pnt.GetY() * 10);
	}

	double GetAngleFrom(Point pnt)
	{
		Point tmpPoint = Point(pnt.GetX() - m_X,  m_Y - pnt.GetY());

		double radian_angle = atan(double(tmpPoint.GetY()) / double(tmpPoint.GetX()));

		return Math::ConvertRadiansToDegrees(radian_angle);
	}

	double GetDistanceFrom(Point pnt)
	{
		return sqrt(pow(m_X - pnt.GetX(), 2) + pow(m_Y - pnt.GetY(), 2));
	}

private:
	int m_X;
	int m_Y;
};

struct Location : public Point
{
public:
	Location(int X, int Y, int YAW) : Point(X, Y), m_YAW(YAW) {}

	int GetYaw() {return m_YAW;}
	void SetYaw(int YAW) { m_YAW = YAW;}

	Location& operator=(Location other) // copy assignment
	{
		if (this != &other) { // self-assignment check expected
			SetX(other.GetX());
			SetY(other.GetY());
			m_YAW = other.GetYaw();
		}

		return *this;
	}

	Location& operator=(Point other) // copy assignment
	{
		if (this != &other) { // self-assignment check expected
			SetX(other.GetX());
			SetY(other.GetY());
			m_YAW = INVALID_YAW;
		}

		return *this;
	}

private:
	int  m_YAW;
};
