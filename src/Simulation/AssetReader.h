#pragma once
#include <iostream>
#include <fstream>
#include <filesystem>
#include <glm/glm.hpp>

#define ASSET_COMPONENT "COMPONENT"
#define ASSET_MESH "MESH"
#define ASSET_MATERIAL "MATERIAL"

namespace AssetReader {

	std::string getBetweenCharacters(std::string, std::string);
	std::string getBetweenCharacters(std::string, std::string, std::string);
	float getFloat(std::string);
	int getInteger(std::string);
	std::string getString(std::string);
	glm::vec3 getVector(std::string);

}