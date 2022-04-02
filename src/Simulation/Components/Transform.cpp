#include "Transform.h"

Transform::Transform()
{
	position =	glm::vec3(0, 0, 0);		// Initial position, the origin
	rotation =	glm::quat(0, 0, 0, 0);	// Initial rotation, quaternion identity
	scale =		glm::vec3(1, 1, 1);		// Initial scale, 1x
}

void Transform::SetEuler(float x, float y, float z)
{
	glm::mat4 rotationMatrix = glm::mat4();
	rotationMatrix = glm::rotate(rotationMatrix, z, glm::vec3(0, 0, 1));
	rotationMatrix = glm::rotate(rotationMatrix, y, glm::vec3(0, 1, 0));
	rotationMatrix = glm::rotate(rotationMatrix, x, glm::vec3(1, 0, 0));
	rotation = glm::toQuat(rotationMatrix);
}

void Transform::SetEuler(glm::vec3 eulerAngles)
{
	SetEuler(eulerAngles.x, eulerAngles.y, eulerAngles.z);
}

void Transform::RotateEuler(float x, float y, float z)
{
	glm::mat4 rotationMatrix = glm::toMat4(rotation);
	rotationMatrix = glm::rotate(rotationMatrix, z, glm::vec3(0, 0, 1));
	rotationMatrix = glm::rotate(rotationMatrix, y, glm::vec3(0, 1, 0));
	rotationMatrix = glm::rotate(rotationMatrix, x, glm::vec3(1, 0, 0));
	rotation = glm::toQuat(rotationMatrix);
}
