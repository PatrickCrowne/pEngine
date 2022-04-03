#include "Camera.h"

void Camera::MoveRelative(glm::vec3 offset)
{

	transform.position += glm::rotate(transform.rotation, offset);

}
