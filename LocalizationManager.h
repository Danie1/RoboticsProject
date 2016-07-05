/*
 * Localization.h
 */

#ifndef LOCALIZATION_H_
#define LOCALIZATION_H_

#include <vector>
#include <libplayerc++/playerc++.h>
using namespace std;
using namespace PlayerCc;
#include "Particle.h"
#include "Graph.h"
#include "Robot.h"

class Map;
class Particle;

class Localization {
private:
	Graph& m_graph;
	Robot& m_robot;
	//Location m_StartLocation;
	//Location m_CurrentLocation;

	float xDelta;
	float yDelta;
	float yawDelta;

	float Random(float min, float max);
	void ChildsToParticles(vector<Particle*> childs);
	void BreedParticle(Particle* particle, int dwChildCount, vector<Particle*>& childs);
	void BreedParticle(Particle* particle, int ChildCount, float dExpansionRadius, float dYawRange, vector<Particle*>& childs);
	//Location GetCurrentLocationFromParticle();


public:
	void PrintParticlesOnMap(const char* szFileName);

	void initParticles(int amount);
	vector<Particle*> particles;
	bool CreateParticles(float xDelta, float yDelta, float yawDelta, float belief, float expansionRadius, float yawRange, int childsCount);
	double eval(Robot& robot, Graph& graph);
	Localization(Graph& map, Robot& robot);
	void Update(double xDelta, double yDelta, double yawDelta, Robot& robot);
	Particle* BestParticle();

};

#endif /* LOCALIZATION_H_ */