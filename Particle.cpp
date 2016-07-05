/*
 * Particles.cpp
 */

#include "Particle.h"
#include <libplayerc++/playerc++.h>
using namespace PlayerCc;
#include <math.h>
#include "Definitions.h"
#include "FilesManager.h"
#include "ConfigurationManager.h"

Particle::Particle(float xDelta, float yDelta, float yawDelta, float belief)
{
	this->xDelta = xDelta;
	this->yDelta = yDelta;
	this->yawDelta = yawDelta;
	this->belief = belief;


	this->age = 1;
}

// Create a new child-particle.
Particle* Particle::CreateChild(float expansionRadius, float yawRange, Graph& map)
{
    float newX = this->xDelta + Random(-expansionRadius, expansionRadius);
    float newY = (this->yDelta + Random(-expansionRadius, expansionRadius));
    float newYaw = this->yawDelta + Random(-yawRange, yawRange);

    while (((newX) < 0|| (newX) >= map.GetWidth() -10 ||
           newY < 0 || (newY) >= map.GetHeight() -10)||
        		   map.IsCellObstacle(newY, newX))
    {
    	newX = this->xDelta + Random(-expansionRadius, expansionRadius);
        newY = (this->yDelta + Random(-expansionRadius, expansionRadius));
    }

    return new Particle(newX, newY, newYaw, 0.51);
}

// Update the particle
void Particle::Update(float xDelta,
					  float yDelta,
					  float yawDelta,
					  Graph& map,
					  Robot& robot)
{
	this->xDelta += xDelta;
    this->yDelta += yDelta;
    this->yawDelta = fmod(this->yawDelta + yawDelta, 360);

    double probByRobotDistance = 1 / sqrt(pow(this->yDelta - robot.GetY(), 2) - pow(this->xDelta - robot.GetX(), 2));

    // if the particle is out of the map or on obstacle
    if ((this->xDelta) < 0 || (this->xDelta) >= map.GetWidth() -10 ||
        this->yDelta < 0 || (this->yDelta) >= map.GetHeight() -10){
    	//printf("Respawn outside the map. \r\n");
    	this->Respawn(map);
    }
    else if (map.IsCellObstacle(this->yDelta, this->xDelta))
    {
    	//printf("Respawn due to obstacle \r\n");
    	this->Respawn(map);
    }
    else
    {
    	// Check if the distance between the particle and the robot makes sense.
		float predictionBelif = ProbabilityByMovement(abs(robot.GetX() - this->xDelta),
													  abs(robot.GetY() - this->yDelta),
													  abs(robot.GetYaw() - this->yawDelta));// * this->belief;

		// Check if the obstacles the robot sees match the particle.
		float probabilityByScan = ProbabilityByLaserScan(map, robot);
		this->belief = probabilityByScan * predictionBelif;
		//printf("predictionBelif = %f. probabilityByScan = %f => Belief: %f \r\n", predictionBelif, probabilityByScan, this->belief);

    }
}

void Particle::Respawn(Graph& map) {
	this->xDelta = (rand() % map.GetWidth());
	this->yDelta = (rand() % map.GetHeight());
	double yaw = rand() % (int)(2*M_PI);
	this->belief = 0.1;
	while (map.IsCellObstacle(this->yDelta, this->xDelta)){
		this->xDelta = (rand() % map.GetWidth());
		this->yDelta = (rand() % map.GetHeight());
	}
}

float Particle::Random(float min, float max)
{
    return (max - min ) * ((float)rand() / (float)RAND_MAX )  + min;
}

// calculate the distance. the more the distance is shorter - the better probability.
float Particle::ProbabilityByMovement(float xDelta, float yDelta, float yawDelta)
{
	float distance = sqrt(pow(xDelta,2) + pow(yDelta,2));

	if (distance < 1)
		return 1;

	if (distance < 3)
		return 0.9;

	if (distance < 5)
		return 0.7;

	if (distance < 7)
		return 0.5;

	if (distance < 9)
		return 0.3;

	return 0.1;
}

// Get the probability of this particle by using the laser scan.
float Particle::ProbabilityByLaserScan(Graph& graph, Robot& robot)
{
	float totalHits = 0;
	float correctHits = 0;

	// Measures the position of the
	for (unsigned int index = 200; index < 400; index++)
	{
		double distance = robot.GetDistanceFromLaser(index) * 10;

		// Takes the distance of the indexed obstacle from the laser (and from the robot)
		// If the laser cannot see an obstacle
		if (distance >=40)
		{
			// let's move to the next sample
			continue;
		}

		totalHits++;

		const double RANGE = LASER_SCOPE;
		const double RATIO = RANGE / LASER_COUNT;
		double StartingDegree = (180 - (360 - RANGE)) / 2 + 180;
		double indexDegree = fmod(StartingDegree + ((double(index) * RATIO)) + 360, 360);
		double obstacleRadian = Math::ConvertDegreesToRadians(fmod(indexDegree + robot.GetYaw() + (180 - (360 - RANGE)) / 2 + 360, 360));

		double obstacleX = distance * cos(obstacleRadian) + xDelta;
		obstacleX = 2*xDelta - obstacleX;
		double obstacleY = distance * sin(obstacleRadian) + yDelta;


		// Check if we missed boundaries.
		if ((obstacleX) < 0 || (obstacleX) >= graph.GetWidth() ||
			 obstacleY < 0 || (obstacleY) >= graph.GetHeight())
		{
			continue;
		}

		// Check if there's a hit on an obstacle.
		if (graph.IsCellObstacle(obstacleY, obstacleX))
		{
			correctHits++;
		}
	}

	if (totalHits == 0)
		return 0;
	else
		return correctHits/totalHits;
}

void Particle::PrintParticleFOV(Graph& graph, Robot& robot)
{
	printf("Particle: (%f, %f, %f) -- Robot: (%f, %f, %f) -- Belief: %f\r\n",
	    		this->xDelta, this->yDelta, this->yawDelta,
	    		robot.GetX(), robot.GetY(), robot.GetYaw(), this->belief);

	Graph printedgraph = graph;

	// Measures the position of the
	for (unsigned int index = 0; index < LASER_COUNT; index++)
	{
		double distance = robot.GetDistanceFromLaser(index) * 10;

		const double RANGE = LASER_SCOPE;
		const double RATIO = RANGE / LASER_COUNT;
		double StartingDegree = (180 - (360 - RANGE)) / 2 + 180;
		double indexDegree = fmod(StartingDegree + ((double(index) * RATIO)) + 360, 360);
		double obstacleRadian = Math::ConvertDegreesToRadians(fmod(indexDegree + robot.GetYaw() + 30 + 360, 360));

		double obstacleX = distance * cos(obstacleRadian) + xDelta;
		obstacleX = 2*xDelta - obstacleX;
		double obstacleY = distance * sin(obstacleRadian) + yDelta;

		bool fMarkAsHit = false;

		// Draw far away pixels as obstacles.
		if (distance >=40)
		{
			fMarkAsHit = true;
		}

		// Check if we missed boundaries.
		if ((obstacleX) < 0 || (obstacleX) >= graph.GetWidth() ||
			 obstacleY < 0 || (obstacleY) >= graph.GetHeight())
		{
			continue;
		}

		// Check if there's a hit on an obstacle.
		if (graph.IsCellObstacle(obstacleY, obstacleX))
		{
			printedgraph.SetCell(obstacleY, obstacleX, ecellState_particle);
		}
		else if(fMarkAsHit)
		{
			printedgraph.SetCell(obstacleY, obstacleX, ecellState_waypoint);
		}
		else
		{
			printedgraph.SetCell(obstacleY, obstacleX, eCellState_finish);
		}

	}
	printedgraph.SetCell(this->yDelta, this->xDelta, eCellState_start);


	Map EnlargedPrintedMap(ConfigurationManager::Configuration().GetMapResolution(),
						   ConfigurationManager::Configuration().GetRobotSize());

	printedgraph.ConvertToMap(EnlargedPrintedMap);

	EnlargedPrintedMap.SaveToFile(FilesManager::Get().GetBestParticleFOVFile().c_str());
}

float Particle::GetX()
{
    return xDelta;
}

float Particle::GetY()
{
    return yDelta;
}

float Particle::GetYaw()
{
    return yawDelta;
}
