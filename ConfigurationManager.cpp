#include "ConfigurationManager.h"
#include "Utils.h"
#include <stdlib.h>

ConfigurationManager::ConfigurationManager() : m_StartLocation(0,0,0), m_EndPoint(0,0)
{
	/**
	 * Get the configuration from the file.
	 */
	if (Utils::getParamsFromFile(PARAMS_FILE_NAME, // Parameters.txt
								m_InputMapFileName, // Where the map file image is
								m_StartLocation, // Where the robot starts
								m_EndPoint, // Where the robot finishes
								m_RobotSize, // The robot size. We assume its a square atm
								m_MapResolution, // The map resolution
								m_GridResolution) == false) // The grid resolution
	{
		printf("Failed to get params from file.\n");
		exit(-1);
	}
}

ConfigurationManager::~ConfigurationManager()
{

}

double ConfigurationManager::GetRobotSize()
{
	return m_RobotSize;
}

double ConfigurationManager::GetMapResolution()
{
	return m_MapResolution;
}

double ConfigurationManager::GetGridResolution()
{
	return m_GridResolution;
}

Location ConfigurationManager::GetStartLocation()
{
	return m_StartLocation;
}

Point ConfigurationManager::GetEndPoint()
{
	return m_EndPoint;
}

const char* ConfigurationManager::GetInputFilename()
{
	return m_InputMapFileName;
}
