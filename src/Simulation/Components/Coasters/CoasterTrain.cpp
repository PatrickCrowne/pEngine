#include "CoasterTrain.h"
#include "../MeshRenderer.h"
#include "../../Simulator.h"
CoasterTrain::CoasterTrain(Train* t) {

	train = t;

}

/// <summary>
/// Updates the train's position along the track
/// </summary>
void CoasterTrain::Update() {

	// The vector that will store the sum of forces on the train
	float sumOfForces = 0;

	for (int i = 0; i < train->cars.size(); i++) {
		// Add the forward vector
		glm::mat4 carMatrix = train->cars[i]->getComponent<MeshRenderer>()->getModelMatrix();
		glm::vec3 forward = glm::normalize(glm::vec3(carMatrix * glm::vec4(0, 0, 1, 0)));
		glm::vec3 gravity = glm::vec3(0, -1, 0);
		sumOfForces += glm::dot(forward, gravity) * 9.81f;
	}
	// Move the train along the track 
	train->velocity += sumOfForces * Simulator::deltaTime;
	train->trackPos += train->velocity * Simulator::deltaTime;

}