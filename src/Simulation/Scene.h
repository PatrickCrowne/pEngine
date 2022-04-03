#pragma once
#include <vector>
#include "SimObjects/Camera.h"
#include "SimObjects/SimObject.h"
class Scene
{
public:
	bool registerCamera(Camera*);
	bool registerSimObject(SimObject*);
	bool updateScene();
	bool fixedUpdateScene();
private:
	std::vector<Camera*> sceneCameras;
	std::vector<SimObject*> sceneObjects;
};

