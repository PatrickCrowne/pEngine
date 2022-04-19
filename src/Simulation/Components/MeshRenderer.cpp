#include "MeshRenderer.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "../Simulator.h"
#include "../../glstate.hpp"

MeshRenderer::MeshRenderer() {

	GLState::registerRenderer(this);

}

MeshRenderer::~MeshRenderer() {

	GLState::unregisterRenderer(this);

}

/// <summary>
/// Called at the start of this Component's life
/// </summary>
void MeshRenderer::Awake() {

	

}

void MeshRenderer::Update() {

	/*std::vector<int> triangles = {0, 1, 3, 0, 3, 2};
	std::vector <glm::vec3> vertices;

	vertices.push_back(glm::vec3(sin(Simulator::time) - 0.5f, 0, 0));
	vertices.push_back(glm::vec3(sin(Simulator::time) + 0.5f, 0, 0));
	vertices.push_back(glm::vec3(cos(Simulator::time) - 0.5f, 0, 1));
	vertices.push_back(glm::vec3(cos(Simulator::time) + 0.5f, 0, 1));

	mesh->updateMesh(vertices, triangles);

	vertices.clear();
	triangles.clear();*/

}



/// <summary>
/// Renders this mesh to the scene
/// </summary>
void MeshRenderer::Render(glm::mat4 viewProjMatrix) {

	// Set the shader
	glUseProgram(material->shader->compiledShaderId);
	// Apply attributes to shader
	material->applyAttributes();

	glm::mat4 modelMatrix = glm::mat4(1);

	modelMatrix = glm::scale(modelMatrix, MeshRenderer::transform->scale);
	modelMatrix = glm::translate(modelMatrix, MeshRenderer::transform->position);
	glm::vec3 eulerAngles = glm::eulerAngles(MeshRenderer::transform->rotation);
	modelMatrix = glm::rotate(modelMatrix, eulerAngles.x, glm::vec3(1, 0, 0));
	modelMatrix = glm::rotate(modelMatrix, eulerAngles.y, glm::vec3(0, 1, 0));
	modelMatrix = glm::rotate(modelMatrix, eulerAngles.z, glm::vec3(0, 0, 1));
	

	if (material->modelMatrixAttributeName.length() > 0)
		glUniformMatrix4fv(material->shader->uniformInputs.at(material->modelMatrixAttributeName), 1, GL_FALSE, glm::value_ptr(modelMatrix));

	if (material->viewProjMatrixAttributeName.length() > 0)
		glUniformMatrix4fv(material->shader->uniformInputs.at(material->viewProjMatrixAttributeName), 1, GL_FALSE, glm::value_ptr(viewProjMatrix));

	if (material->cameraPositionAttributeName.length() > 0)
		glUniform3fv(material->shader->uniformInputs.at(material->cameraPositionAttributeName), 1, glm::value_ptr(GLState::camPos));

	if (material->modelPositionAttributeName.length() > 0)
		glUniform3fv(material->shader->uniformInputs.at(material->modelPositionAttributeName), 1, glm::value_ptr(-MeshRenderer::transform->position));

	// Draw the mesh
	mesh->draw();
	// Cleanup state
	glBindVertexArray(0);

}