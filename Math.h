#pragma once
#include <math.h>

class Math
{
public:
	static double ConvertRadiansToDegrees(double rad)
	{
		return fmod((rad * 180) / M_PI + 180, 360);
	}

	static double ConvertDegreesToRadians(double deg)
	{
		return ((deg + 180) * M_PI) / 180;
	}
};
