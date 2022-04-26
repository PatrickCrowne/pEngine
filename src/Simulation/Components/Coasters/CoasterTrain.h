#pragma once
#include "../Component.h"
#include "../../SimObjects/Train.h"
class CoasterTrain : public Component
{

public:
	CoasterTrain(Train*);
	void Update();

private:
	Train* train;

};

