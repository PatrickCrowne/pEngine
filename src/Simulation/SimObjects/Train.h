#pragma once
#include <vector>
#include "Car.h"
#include "SimObject.h"
#include "../Components/Coasters/TrackSpline.h"
class Train : public SimObject
{

public:
	Train(float, int, TrackSpline*);
	~Train() {}

	std::vector<Car*> cars;
	float trackPos = 0;
	float carSpacing = 3.2f;
	float velocity = 0;

private:
	void addCar();

};

