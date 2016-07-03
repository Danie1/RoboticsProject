#include "FilesManager.h"

FilesManager& FilesManager::Get()
{
	static FilesManager fm;
	return fm;
}

std::string FilesManager::GetNextParticleFile()
{
	char buff[100];
	snprintf(buff, sizeof(buff), "%s%d%s", "Images/ParticleStage", m_NextParticle, ".png");
	std::string buffAsStdStr = buff;

	m_NextParticle++;

	return buffAsStdStr;
}

std::string FilesManager::GetMapRouteFile()
{
	return std::string("Images/EnlargedRouteImage.png");
}

std::string FilesManager::GetGraphRouteFile()
{
	return std::string("Images/RouteImage.png");
}

std::string FilesManager::GetBestParticleFOVFile()
{
	char buff[100];
	snprintf(buff, sizeof(buff), "%s%d%s", "Images/BestParticleStage", m_BestNextParticle, ".png");
	std::string buffAsStdStr = buff;

	m_BestNextParticle++;

	return buffAsStdStr;
}
