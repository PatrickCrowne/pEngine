#pragma once
#include <vector>
#include "Car.h"
#include "SimObject.h"
class Train : public SimObject
{

public:
	Train(float, int);
	~Train() {}

	std::vector<Car*> cars;
	float trackPos = 0;
	float carSpacing = 1.5f;
	float velocity = 0;

private:
	void addCar();

};

