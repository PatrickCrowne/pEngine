#pragma once
#include <iostream>
#include <vector>
#include "Components/Component.h"
class SimObject
{
public:
	SimObject();
	~SimObject();
	bool updateComponents();
	bool fixedUpdateComponents();
	std::string name;
	std::vector<Component> components;
};

