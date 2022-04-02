#include "simobject.h"
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <filesystem>
#include "../../util.hpp"
#include "../AssetReader.h"
#include "../ComponentRegistry.h"
#include "../Components/MeshRenderer.h"

SimObject::SimObject() {

}

/// <summary>
/// Called upon the creation of a new SimObject
/// </summary>
SimObject::SimObject(std::string simObjectConfigFile) {

	std::ifstream simObjectConfigFileStream;
	simObjectConfigFileStream.open(simObjectConfigFile);

	std::string buffer;
	// Read all the lines of the material configuration file
	while (getline(simObjectConfigFileStream, buffer)) {

		if (buffer._Starts_with(SIMOBJECT_NAME)) {
			name = AssetReader::getBetweenCharacters(buffer, "\"");
			continue;
		}

		if (buffer._Starts_with(TRANSFORM_POSITION)) {
			transform.position = AssetReader::getVector(buffer);
			continue;
		}

		if (buffer._Starts_with(TRANSFORM_ROTATION)) {
			transform.SetEuler(AssetReader::getVector(buffer));
			continue;
		}

		if (buffer._Starts_with(TRANSFORM_SCALE)) {
			transform.scale = AssetReader::getVector(buffer);
			continue;
		}

		// Component handling
		if (buffer._Starts_with(ASSET_COMPONENT)) {

			// Get the component type
			std::string componentType = AssetReader::getBetweenCharacters(buffer, " ");
			
			// TODO: Write this procedurally
			if (componentType._Equal("MeshRenderer")) {
				MeshRenderer* meshRenderer = new MeshRenderer();
				addComponent(meshRenderer);

				getline(simObjectConfigFileStream, buffer);
				while (!buffer._Starts_with("}")) {

					if (buffer._Starts_with("\tMESH")) {
						meshRenderer->mesh = new Mesh(AssetReader::getString(buffer));
					}

					if (buffer._Starts_with("\tMATERIAL")) {
						meshRenderer->material = Material::getMaterial(AssetReader::getString(buffer));
					}

					getline(simObjectConfigFileStream, buffer);
				}

			}

			
			continue;
		}

	}

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