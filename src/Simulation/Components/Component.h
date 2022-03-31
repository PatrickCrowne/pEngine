#pragma once
#include "Transform.h"
class Component
{
public:
	Transform* transform;
	bool active;
	virtual void Awake();
	virtual void Update();
	virtual void FixedUpdate();
	virtual void Destroy();
};

