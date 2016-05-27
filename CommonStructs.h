#pragma once

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
