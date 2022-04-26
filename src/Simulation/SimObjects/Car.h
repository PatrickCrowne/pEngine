#pragma once
#include "SimObject.h"
class Car : public SimObject
{

public:
	Car(float);
	~Car() {}

private:
	float carOffset;

};

