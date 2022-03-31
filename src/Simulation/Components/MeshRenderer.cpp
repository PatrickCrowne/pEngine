#include "MeshRenderer.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
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

	// Scale and center mesh using its bounding box
	/*auto meshBB = mesh->boundingBox();
	glm::mat4 fixBB = glm::scale(glm::mat4(1.0f),
		glm::vec3(1.0f / glm::length(meshBB.second - meshBB.first)));
	fixBB = glm::translate(fixBB, -(meshBB.first + meshBB.second) / 2.0f);
	// Concatenate bounding box transform to view transform and upload to shader
	viewProjMatrix = viewProjMatrix * fixBB;*/

	viewProjMatrix = glm::scale(viewProjMatrix, MeshRenderer::transform->scale);
	viewProjMatrix *= glm::toMat4(MeshRenderer::transform->rotation);
	viewProjMatrix = glm::translate(viewProjMatrix, MeshRenderer::transform->position);

	glUniformMatrix4fv(material->shader->uniformInputs.at(material->modelMatrixAttributeName), 1, GL_FALSE, glm::value_ptr(viewProjMatrix));

	

	// Draw the mesh
	mesh->draw();
	// Cleanup state
	glBindVertexArray(0);

}