#pragma once

#include <cmath>
#include <math.h>
#include "Math.h"
#include <stdio.h>

const int INVALID_YAW = -1;

struct Point
{
public:
	Point(double X, double Y) { m_X = X; m_Y = Y;}

	double GetX() { return m_X;}
	double GetY() { return m_Y; }
	void SetX(double X) { m_X = X;	}
	void SetY(double Y) { m_Y = Y;	}

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

		double degree_angle = Math::ConvertRadiansToDegrees(radian_angle);

		// If the new point is in first or forth quarter, fix the angle
		if (tmpPoint.GetX() > 0)
		{
			degree_angle = fmod(degree_angle + 180, 360);
		}

		return degree_angle;
	}

	double GetDistanceFrom(Point pnt)
	{
		return sqrt(pow(m_X - pnt.GetX(), 2) + pow(m_Y - pnt.GetY(), 2));
	}

private:
	double m_X;
	double m_Y;
};

struct Location : public Point
{
public:
	Location(double X, double Y, int YAW) : Point(X, Y), m_YAW(YAW) {}

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
