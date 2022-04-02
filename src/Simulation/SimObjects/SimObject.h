#pragma once
#include <iostream>
#include <vector>
#include "../Components/Component.h"
#include "../Components/Transform.h"
#include <typeinfo>

#define SIMOBJECT_NAME "SIMOBJECT_NAME"
#define TRANSFORM_POSITION "TRANSFORM_POSITION"
#define TRANSFORM_ROTATION "TRANSFORM_ROTATION"
#define TRANSFORM_SCALE "TRANSFORM_SCALE"

class SimObject
{
public:
	SimObject::SimObject();
	SimObject(std::string);
	~SimObject();
	bool updateComponents();
	bool fixedUpdateComponents();
	void addComponent(Component*);
	void removeComponent(Component*);
	template <typename T>
	T* getComponent() {
		for (Component* c : components) {
			if (dynamic_cast<T*>(c) != nullptr) {
				return dynamic_cast<T*>(c);
			}
		}
		return NULL;
	}
	std::string name;
	std::vector<Component*> components;
	Transform transform;
};

