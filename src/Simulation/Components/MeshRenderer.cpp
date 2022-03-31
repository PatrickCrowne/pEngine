#include "MeshRenderer.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "../Simulator.h"
/// <summary>
/// Called at the start of this Component's life
/// </summary>
void MeshRenderer::Awake() {


}

/// <summary>
/// Renders this mesh to the scene
/// </summary>
void MeshRenderer::Render(glm::mat4 viewProjMatrix) {

	// Set the shader
	glUseProgram(material->shader->compiledShaderId);
	// Apply attributes to shader
	material->applyAttributes();

	viewProjMatrix = glm::scale(viewProjMatrix, MeshRenderer::transform->scale);
	viewProjMatrix *= glm::toMat4(MeshRenderer::transform->rotation);
	viewProjMatrix = glm::translate(viewProjMatrix, MeshRenderer::transform->position);

	glUniformMatrix4fv(material->shader->uniformInputs.at(material->modelMatrixAttributeName), 1, GL_FALSE, glm::value_ptr(viewProjMatrix));

	

	// Draw the mesh
	mesh->draw();
	// Cleanup state
	glBindVertexArray(0);

}