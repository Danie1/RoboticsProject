#pragma once
#include <math.h>

class Math
{
public:
	static double ConvertRadiansToDegrees(double rad)
	{
		return (rad * 180) / M_PI + 180;
	}

	static double ConvertDegreesToRadians(double deg)
	{
		return (deg * M_PI) / 180;
	}
};
