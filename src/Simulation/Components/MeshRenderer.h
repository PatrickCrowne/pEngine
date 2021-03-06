#pragma once
#include "Component.h"
#include "../Material.h"
#include "../mesh.hpp"

class MeshRenderer : public Component
{
public:
	MeshRenderer();
	~MeshRenderer();
	Mesh *mesh;
	Material *material;
	void Awake();
	void Render(glm::mat4);
	void Update();
	bool shadowCaster = true;
	glm::mat4 MeshRenderer::getModelMatrix();
	glm::mat4 lightSpaceMat;
};

