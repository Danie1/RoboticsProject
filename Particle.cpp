/*
 * Particles.cpp
 */

#include "Particle.h"
#include <libplayerc++/playerc++.h>
using namespace PlayerCc;
#include <math.h>
#include "Definitions.h"

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
    this->yawDelta += yawDelta;

    double probByRobotDistance = 1 / sqrt(pow(this->yDelta - robot.GetY(), 2) - pow(this->xDelta - robot.GetX(), 2));

    // if the particle is out of the map or on obstacle
    if ((this->xDelta) < 0 || (this->xDelta) >= map.GetWidth() -10 ||
        this->yDelta < 0 || (this->yDelta) >= map.GetHeight() -10){
    	printf("Respawn outside the map. \r\n");
    	this->Respawn(map);
    }
    else if (map.IsCellObstacle(this->yDelta, this->xDelta))
    {
    	printf("Respawn due to obstacle \r\n");
    	this->Respawn(map);
    }
    else
    {
    	// Calculating the belief of the particle, by using the probability by movement and laser scan.
		float predictionBelif = ProbabilityByMovement(xDelta, yDelta, yawDelta);// * this->belief;
		float probabilityByScan = ProbabilityByLaserScan(this->xDelta, this->yDelta, this->yawDelta, map, robot);
		printf("predictionBelif = %f. probabilityByScan = %f\n", predictionBelif, probabilityByScan);
		this->belief = probabilityByScan * predictionBelif * BELIEF_MAGIC_NUMBER;
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
float Particle::ProbabilityByLaserScan(float xRobotDelta,
		float yRobotDelta,
		float yawRobotDelta,
		Graph& graph,
		Robot& robot)
{
	float totalHits = 0;
	float correctHits = 0;

	Graph gr = graph;

	// Measures the position of the
	for (unsigned int index = 0; index < LASER_COUNT; index++)
	{
		double distance = robot.GetDistanceFromLaser(index) * 10;

		// Takes the distance of the indexed obstacle from the laser (and from the robot)
		// If the laser cannot see an obstacle
		if (distance >=40)
		{
			// let's move to the next sample
//			continue;
		}

		totalHits++;
		const double RANGE = 240;
		const double RATIO = RANGE / 666;
		double StartingDegree = 360 - (180 - (360 - RANGE)) / 2;
		double indexDegree = fmod(StartingDegree + ((double(index) * RATIO)) + 360, 360);
		//double indexDegree = (index) * 0.36 - 120;
		//double indexRadian = (indexDegree) *M_PI / 180;
		//double obstacleRadian = indexRadian + Math::ConvertDegreesToRadians(robot.GetYaw());
		double obstacleRadian = Math::ConvertDegreesToRadians(fmod(indexDegree + robot.GetYaw() + 360, 360));

		double obstacleX = distance * cos(obstacleRadian) + xDelta;
		double obstacleY = distance * sin(obstacleRadian) + yDelta;

		gr.SetCell(obstacleY, obstacleX, ecellState_particle);

		// Check if we missed boundaries.
		if ((obstacleX) < 0 || (obstacleX) >= graph.GetWidth() -10 ||
			 obstacleY < 0 || (obstacleY) >= graph.GetHeight() -10)
		{
			continue;
		}

		// Check if there's a hit on an obstacle.
		if (graph.IsCellObstacle(obstacleY, obstacleX))
		{
			correctHits++;
		}

	}

	gr.SaveToFile("OrGafni.png");
	if (totalHits == 0)
		return 0;
	else
		return correctHits/totalHits;

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
