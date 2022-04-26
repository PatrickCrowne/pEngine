#include "Scene.h"

bool Scene::registerCamera(Camera *cam)
{
	sceneCameras.push_back(cam);
	return true;
}

bool Scene::registerSimObject(SimObject *simObj)
{
	sceneObjects.push_back(simObj);
	return true;
}

/// <summary>
/// Updates the scene each frame
/// </summary>
/// <returns></returns>
bool Scene::updateScene() {

	// Itrate through scene object array and update all components
	for (SimObject *simObj : Scene::sceneObjects) {
		simObj->updateComponents();
		simObj->update();
	}
	return true;

}

bool Scene::fixedUpdateScene() {

	// Itrate through scene object array and update all components
	for (SimObject *simObj : Scene::sceneObjects) {
		simObj->fixedUpdateComponents();
	}
	return true;

}