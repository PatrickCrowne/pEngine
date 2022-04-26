#pragma once
#include "SimObject.h"
#include "Train.h"
#include "../Scene.h"
#include <vector>
#include "../Components/Coasters/TrackSpline.h"
#include "CoasterStyle.h"



class Coaster : public SimObject
{

public:
	Coaster(std::string, std::string, Scene*);
	~Coaster();

	CoasterStyle* coasterStyle;

	TrackSpline trackSpline;
private:
	std::vector<Train*> trains;
	void parseStyle(std::string);
	

};

