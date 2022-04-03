#pragma once
#include "SimObject.h"

#define CAMERA_PITCH_MAX 89.0f
#define CAMERA_PITCH_MIN 89.0f

class Camera : public SimObject
{
public:
	void MoveRelative(glm::vec3 offset);
	void SetPosition(glm::vec3 newPosition);
	void Rotate(glm::vec3 eulerAngles);
	void SetRotation(glm::vec3 eulerAngles);
};

