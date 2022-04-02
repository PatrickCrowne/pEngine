#pragma once
#include "SimObject.h"
#include "../Components/MeshRenderer.h"
#include "../mesh.hpp"
class TrackMeshSegment : public SimObject
{

public:
	TrackMeshSegment();
	Mesh* getMesh();
protected:
	MeshRenderer *trackMeshRenderer;

};

