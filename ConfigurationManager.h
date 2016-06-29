#pragma once
#include "Definitions.h"
#include "CommonStructs.h"

/**
 * This class manages the configuration in the program. It works as a singleton.
 */
class ConfigurationManager
{
private:
	ConfigurationManager();
	~ConfigurationManager();

public:
	static ConfigurationManager& Configuration()
	{
		static ConfigurationManager conf;
		return conf;
	}

	double GetRobotSize();
	double GetMapResolution();
	double GetGridResolution();
	Location GetStartLocation();
	Point GetEndPoint();
	const char* GetInputFilename();

private:
	char     m_InputMapFileName[MAX_FILE_PATH];
	double   m_MapResolution;
	double   m_GridResolution;
	int      m_RobotSize;
	Location m_StartLocation;
	Point    m_EndPoint;
};
