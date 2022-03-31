#pragma once
class Component
{
public:
	bool active;
	void Awake();
	void Update();
	void FixedUpdate();
	void Destroy();
};

