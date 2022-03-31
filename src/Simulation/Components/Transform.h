#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
class Transform
{
public:
	Transform();
	void RotateEuler(float, float, float);
	void SetEuler(float, float, float);
	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;
};

