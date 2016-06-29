#pragma once
#include <string>

/**
 * The sole purpose of this class is to return the
 * path of the designated file for writing.
 */
class FilesManager
{
private:
	FilesManager(){}
	~FilesManager(){}

public:
	static FilesManager& Get();

	std::string GetNextParticleFile();

	std::string GetMapRouteFile();

	std::string GetGraphRouteFile();

private:
	int m_NextParticle;
};
