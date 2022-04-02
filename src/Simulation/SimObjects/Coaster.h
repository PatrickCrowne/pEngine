#pragma once
#include "SimObject.h"
#include "../Components/Coasters/TrackSpline.h"
class Coaster : public SimObject
{

public:
	Coaster();
	~Coaster();

	TrackSpline trackSpline;

private:


};

