#pragma once
#include "Component.h"
#include "../Material.h"
#include "../mesh.hpp"

class MeshRenderer : public Component
{
public:
	Mesh *mesh;
	Material *material;
	void Awake();
	void Render(glm::mat4);
	void Update();
};

