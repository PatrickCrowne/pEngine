#pragma once
#include "../Component.h"
#include "../../SimObjects/Train.h"
#include "../Coasters/TrackSpline.h"
class CoasterTrain : public Component
{

public:
	CoasterTrain(Train*, TrackSpline*);
	void Update();

private:
	Train* train;
	TrackSpline* spline;

};

