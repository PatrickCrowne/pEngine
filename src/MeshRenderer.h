#pragma once
#include "Component.h"
#include "mesh.hpp"

class MeshRenderer : public Component
{
public:
	Mesh mesh;
	void Awake();
};

