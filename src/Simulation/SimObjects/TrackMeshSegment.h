#pragma once
#include "SimObject.h"
#include "../Components/MeshRenderer.h"
#include "../mesh.hpp"
class TrackMeshSegment : public SimObject
{

public:
	TrackMeshSegment(int i);
	Mesh* getMesh();
	float length;
	int index;
	glm::vec3 startNormal;
	glm::vec3 endNormal;
protected:
	MeshRenderer *trackMeshRenderer;

};

