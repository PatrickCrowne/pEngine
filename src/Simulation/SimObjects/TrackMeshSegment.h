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
protected:
	MeshRenderer *trackMeshRenderer;

};

