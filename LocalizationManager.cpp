/*
 * Localization.cpp
 */

#include "LocalizationManager.h"
#include "ConfigurationManager.h"

#include <ctime>

Localization::Localization(Graph& graph) : m_graph(graph)
{
	xDelta = yDelta = yawDelta = 0;
}

// Create a new particle.
bool Localization::CreateParticles(float xDelta,
								   float yDelta,
								   float yawDelta,
							   	   float belief,
								   float expansionRadius,
								   float yawRange,
								   int childsCount)
{
	// In case we havn't reached the maximum particles allowed number, create a new particle.
	if (particles.size() + childsCount < MAX_PARTICLES_COUNT)
	{
		Particle* particle = new Particle(xDelta, yDelta, yawDelta, belief);
		particles.push_back(particle);
		vector<Particle*> childs;
		BreedParticle(particle, childsCount, expansionRadius, yawRange, childs);
		ChildsToParticles(childs);
		return true;
	}

	return false;
}

void Localization::initParticles(int amount){
	for (int i = 0; i < amount; i++){
		double x = (rand() % m_graph.GetWidth());
		double y = -(rand() % m_graph.GetHeight());
		double yaw = rand() % (int)(2*M_PI);

		while (m_graph.IsCellObstacle(y,x))
		{
			x = (rand() % m_graph.GetWidth());
			y = (rand() % m_graph.GetHeight());
		}

		Particle* p =new Particle(x, y,yaw,1);
		particles.push_back(p);
	}
}

float Localization::Random(float min, float max)
{
    return (max - min ) * ((float)rand() / (float)RAND_MAX )  + min;
}

double Localization::eval(Robot& robot, Graph& graph)
{
	double sum = 0;
	double dx;
	double dy;
	double err;

	for (int i = 0; i < particles.size();i++){
		dx = particles[i]->xDelta - robot.GetX() + (graph.GetWidth() / 2);
		dy = particles[i]->yDelta - robot.GetY() + (graph.GetHeight() / 2);
		err = sqrt(dx*dx + dy*dy);
		sum += err;
	}

	return(sum /particles.size());

}

// Update the particles' in the vector.
void Localization::Update(double deltaX,
						  double deltaY,
						  double deltaYaw,
						  Robot& robot)
{

	vector<int> childsToRemove;
	int particlesSize = particles.size();

	Particle* bestP = particles[0];

	for (int i = 0; i < particlesSize; i++)
	{

		Particle* particle = particles[i];
		printf("handle particle index %d (%f,%f)\n", i, particle->GetX(), particle->GetY());
		particle->Update(deltaX, deltaY, deltaYaw, m_graph, robot);
		float belif = particle->belief;

		printf("particle belif %f\n", belif);

		// If belief is too low - remove the particle
		if ((belif <= LOW_BELIEF_MIN && particles.size() != 0) || belif == 0)
		{
			printf("particle removed\n");
			childsToRemove.push_back(i);
		}
		// If belief is high - high breed
		else if (belif >= HIGH_BELIEF_MIN &&
				 particles.size() - childsToRemove.size() < MAX_PARTICLES_COUNT)
		{
			printf("belif high.\n");

			Particle* newParticle = particle->CreateChild(1, 1, m_graph);
			printf("newParticle created (%f,%f).\n", newParticle->GetX(), newParticle->GetY());
			particles.push_back(newParticle);

			newParticle = particle->CreateChild(1, 1, m_graph);
			printf("newParticle created (%f,%f).\n", newParticle->GetX(), newParticle->GetY());
			particles.push_back(newParticle);
		}
		// If belief is normal - normal breed
		else if (particles.size() - childsToRemove.size() < MAX_PARTICLES_COUNT)
		{
			printf("belif normal.\n");
			particle->age++;

			Particle* newParticle = particle->CreateChild(10, 2, m_graph);
			printf("newParticle created (%f,%f).\n", newParticle->GetX(), newParticle->GetY());
			particles.push_back(newParticle);
		}

		if (belif > bestP->belief)
		{
			bestP = particles[i];
		}
	}

	printf("removing %d particles/n", childsToRemove.size());
//	 Removing the useless particles.
	if (childsToRemove.size() > 0)
	{
		for(int i = childsToRemove.size() - 1; i >=0 ; i--)
		{
			int indexToRemove = childsToRemove[i];
			particles.erase(particles.begin() + indexToRemove);
		}
	}

	printf("new particles!!/n");
	// if we removed too many particles add some
	while(particles.size() < MAX_PARTICLES_COUNT) {
		Particle* newParticle = bestP->CreateChild(20, 3, m_graph);
		printf("newParticle created (%f,%f).\n", newParticle->GetX(), newParticle->GetY());
		particles.push_back(newParticle);
	}
}

// Create new children-particles, by using the best particles.
void Localization::BreedParticle(Particle* particle, int childCount, vector<Particle*>& childs) {

	// In case we havn't reached the maximum particles allowed number, create a new particle.
	for (int i = 0; i < childCount && particles.size() + i < MAX_PARTICLES_COUNT; i++)
			{
			Particle* child = particle->CreateChild(5, 1, m_graph);
			childs.push_back(child);
		}

}

// Create new children-particles, by using the best particles.
void Localization::BreedParticle(Particle* particle, int childCount, float expansionRadius, float yawRange, vector<Particle*>& childs)
{
	// In case we havn't reached the maximum particles allowed number, create a new particle.

		// Create new child-particles, and push them into the vector.
		for (int i = 0; i < childCount && particles.size() + i < MAX_PARTICLES_COUNT; i++)
		{
			Particle* child = particle->CreateChild(expansionRadius, yawRange, m_graph);
				childs.push_back(child);
		}

}

// This method return the particle with highest belief
Particle* Localization::BestParticle()
{

	// Put the first one as the best just for comparison
	Particle* bestParticle = particles[0];

	cout << "*** Choosing best particle ***" << endl;

	// Search for the best particale by it's belief.
	for (int i = 1; i < particles.size(); i++)
	{
		// If the belief of the current particle is bigger than the biggest we found
		if ((particles[i]->belief) > (bestParticle->belief))
				//&& particles[i]->age < bestParticle->age)
		{
			bestParticle = particles[i];
		}
	}

	xDelta = bestParticle->xDelta;
	yDelta = bestParticle->yDelta;
	yawDelta = bestParticle->yawDelta;

	return bestParticle;
}

void Localization::ChildsToParticles(vector<Particle*> childs)
{
	for (int i = 0;
		particles.size() < MAX_PARTICLES_COUNT && i<childs.size();
		i++)
	{
		particles.push_back(childs[i]);
	}
}

void Localization::PrintParticlesOnMap(const char* szFileName)
{
	Graph printedGraph = m_graph;

	for (int i = 0; i < particles.size(); ++i)
	{
		printedGraph.SetCell(particles[i]->GetY(), particles[i]->GetX(), ecellState_particle);
	}

	Map EnlargedPrintedMap(ConfigurationManager::Configuration().GetMapResolution(),
						   ConfigurationManager::Configuration().GetRobotSize());

	printedGraph.ConvertToMap(EnlargedPrintedMap);

	EnlargedPrintedMap.SaveToFile(szFileName);
}
