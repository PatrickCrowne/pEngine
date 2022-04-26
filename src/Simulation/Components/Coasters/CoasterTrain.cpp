#include "CoasterTrain.h"
#include "../MeshRenderer.h"
#include "../../Simulator.h"
CoasterTrain::CoasterTrain(Train* t, TrackSpline* s) {

	train = t;
	spline = s;

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
		glm::vec3 forward = glm::normalize(glm::vec3(carMatrix * glm::vec4(0, 0, -1, 0)));
		glm::vec3 gravity = glm::vec3(0, -1, 0);
		sumOfForces += glm::dot(forward, gravity) * 9.81f / 4;
	}
	// Move the train along the track 
	train->velocity += sumOfForces * Simulator::deltaTime;
	// Rolling Resistance
	train->velocity -= train->velocity * Simulator::deltaTime * 0.025f;
	// Update track position
	train->trackPos += train->velocity * Simulator::deltaTime;

	// If we reach the end, reset
	if (train->trackPos > spline->length()) {
		train->trackPos = 0;
	}

	// Force minimum velocity of 5u/s
	if (train->velocity < 5) train->velocity = 5;

	// Update position and rotation for each coaster car
	for (int i = 0; i < train->cars.size(); i++) {
		train->cars[i]->transform.position = spline->getPosition(train->trackPos + train->cars[i]->carOffset);

		glm::vec3 pos = train->cars[i]->transform.position;
		train->cars[i]->transform.rotation = spline->getRotation(train->trackPos + train->cars[i]->carOffset);
	}

}