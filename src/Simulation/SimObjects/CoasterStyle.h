#pragma once
#include "SimObject.h"

struct railPos {
	glm::vec3 offset;
	float radius;
};

class CoasterStyle
{

public:
	std::vector<struct railPos> railPos;
	std::string crosstieModel;
	std::string frontCar;
	std::string middleCar;

};

