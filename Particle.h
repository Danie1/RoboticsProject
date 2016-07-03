/*
 * Particle.h
 */

#ifndef PARTICLES_H_
#define PARTICLES_H_
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "Robot.h"
#include <math.h>
#include "Graph.h"

#include <libplayerc++/playerc++.h>
using namespace PlayerCc;
using namespace std;


class Particle {
public:
	float xDelta;
	float yDelta;
	float yawDelta;
	float belief;
	int age;

	void PrintParticleFOV(Graph& graph, Robot& robot);

	float Random(float min, float max);
	float ProbabilityByMovement(float xDelta, float yDelta, float yawDelta);
	float ProbabilityByLaserScan(Graph& graph, Robot& robot);

	void Respawn(Graph& map);
	Particle(float xDelta, float yDelta, float yawDelta, float belief);
	Particle* CreateChild();
	Particle* CreateChild(float expansionRadius, float yawRange,  Graph& map);
	void Update(float X_delta, float Y_delta, float Yaw_delta, Graph& map, Robot& rob);

	float GetX();
	float GetY();
	float GetYaw();
};

#endif /* PARTICLE_H_ */
