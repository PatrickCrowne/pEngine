#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
class Transform
{
public:
	Transform();
	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;
};

