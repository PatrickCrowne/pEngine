#include "Train.h"
#include "../Components/Coasters/CoasterTrain.h"
Train::Train(float trackPosition, int carCount) {

	for (int i = 0; i < carCount; i++) {
		addCar();
	}

	trackPos = trackPosition;

	addComponent(new CoasterTrain(this));

}

/// <summary>
/// Adds a new car to this coaster train
/// </summary>
void Train::addCar() {

	float offset = carSpacing * cars.size();
	Car* car = new Car(offset);
	cars.push_back(car);

}