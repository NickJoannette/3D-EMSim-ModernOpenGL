#ifndef PHYSICSHANDLER_H
#define PHYSICSHANDLER_H

#include <vector>
#define GLM_ENABLE_EXPERIMENTAL

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include "entity.h"
#include <ctime>

//Extern variables used
std::vector <cubicEntity> cubic_entities;

namespace Physics
{
	glm::vec3 dff;
	glm::vec3 mgf;

	float mgdst;
	float mgfrc;
	glm::vec3 sumForces;

	void handleElectroStaticForcesOnEntities()
	{
		sumForces = glm::vec3(0);
		int worldBoundary = 800;
		// At the end of this loop below, each entity has the net electrostatic force summed using Coulomb's law from all other entities acting on it calculated.
		for (int i = 0; i < cubic_entities.size(); ++i)
		{
			// Destroys objects which are out of range by 1100 units from the central position (0,0,0)
			if (abs(cubic_entities[i].Position()->x) >= worldBoundary || abs(cubic_entities[i].Position()->z) >= worldBoundary || abs(cubic_entities[i].Position()->z) >= worldBoundary)
			{
				cubic_entities.erase(cubic_entities.begin() + i);
				continue;
			}
			// reset force sum to 0 for each entity.

			// For every entity
			for (int j = 0; j < cubic_entities.size(); ++j)
			{
				//	Compare it with all other cubic_entities, except itself, and calculate the attractive/repulsive forces incurred
				if (j != i)
				{
					mgdst = glm::distance(*cubic_entities[i].Position(), *cubic_entities[j].Position());
					// Barnes-hut idea: if the objects are too far apart (> 300 units) ignore the effect;
				//  if (mgdst > 75) continue;
					mgfrc = (cubic_entities[i].charge)*(cubic_entities[j].charge) / pow(mgdst, 2);		//magnitude of force felt by both = q1*q2 / sqr(distance)
					sumForces += glm::normalize(*cubic_entities[i].Position() - *cubic_entities[j].Position()) * mgfrc; // Add this resultant vector to the list of force vectors acting on entity i
				}
			}

			// Add the sum of the forces felt by each entity to that entity
			*cubic_entities[i].NetForce() = sumForces;
			// Determine the velocity/acceleration of the object by using F = m*a
			*cubic_entities[i].Velocity() += sumForces / cubic_entities[i].mass;
			sumForces = glm::vec3(0);
		}
	}
}




#endif