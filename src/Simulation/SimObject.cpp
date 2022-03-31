#include "simobject.h"
#include <stdlib.h>
#include <iostream>

/// <summary>
/// Called upon the creation of a new SimObject
/// </summary>
SimObject::SimObject() {

	std::cout << "Created a new Simobject!" << "\n";

}

/// <summary>
/// Called upon the destruction of a SimObject
/// </summary>
SimObject::~SimObject() {

	std::cout << "Destroyed a SimObject!" << "\n";

}

/// <summary>
/// Adds a component to this simObject
/// </summary>
/// <param name="newComponent"></param>
void SimObject::addComponent(Component* newComponent) {
	SimObject::components.push_back(newComponent);
	newComponent->transform = &transform;
}

/// <summary>
/// Removes a component from this simObject
/// </summary>
/// <param name="componentToRemove"></param>
void SimObject::removeComponent(Component* componentToRemove) {
	SimObject::components.erase(std::find(SimObject::components.begin(), SimObject::components.end(), componentToRemove));
}
/// <summary>
/// Updates all the components of this simObject
/// </summary>
bool SimObject::updateComponents() {

	// For each component in this simobject's components
	for (Component *component : SimObject::components) {
		component->Update();
	}
	return true;
}

/// <summary>
/// Updates all the components of this simObject
/// </summary>
bool SimObject::fixedUpdateComponents() {

	// For each component in this simobject's components
	for (Component *component : SimObject::components) {
		component->FixedUpdate();
	}
	return true;
}