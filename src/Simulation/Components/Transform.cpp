#include "Transform.h"

Transform::Transform()
{
	position =	glm::vec3(0, 0, 0);		// Initial position, the origin
	rotation =	glm::quat(0, 0, 0, 0);	// Initial rotation, quaternion identity
	scale =		glm::vec3(1, 1, 1);		// Initial scale, 1x
}
