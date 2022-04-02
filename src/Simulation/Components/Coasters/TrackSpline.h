#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "../../SimObjects/SimObject.h"

class TrackSpline
{

public:
	bool addNode(glm::vec3);
	bool removeNode(int);
	bool buildTrackMesh();

private:
	bool buildTrackMeshSection(int index);
	std::vector<glm::vec3> nodes;
	std::vector<SimObject*> trackModels;

};

