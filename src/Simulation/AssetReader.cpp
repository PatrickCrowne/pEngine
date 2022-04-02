#include "AssetReader.h"
#include <string>
#include "../util.hpp"

/// <summary>
	/// Returns the information between the quotes in the given line
	/// </summary>
	/// <param name="line"></param>
	/// <returns></returns>
std::string AssetReader::getBetweenCharacters(std::string line, std::string c) {
	size_t firstIndex = line.find(c) + 1; // Stores the index of the first quotation mark
	return line.substr(firstIndex, line.find_last_of(c) - firstIndex);
}

std::string AssetReader::getBetweenCharacters(std::string line, std::string c, std::string c2) {
	size_t firstIndex = line.find(c) + 1; // Stores the index of the first quotation mark
	return line.substr(firstIndex, line.find_last_of(c2) - firstIndex);
}

float AssetReader::getFloat(std::string line) {
	return stof(getBetweenCharacters(line, "(", ")"));
}

int AssetReader::getInteger(std::string line) {
	return stoi(getBetweenCharacters(line, "(", ")"));
}

std::string AssetReader::getString(std::string line) {
	return getBetweenCharacters(line, "\"");
}

std::string AssetReader::getTexture(std::string line) {
	return getBetweenCharacters(line, "(", ")");
}

glm::vec3 AssetReader::getVector(std::string line) {
	std::string interior = getBetweenCharacters(line, "(", ")");

	size_t pos = interior.find(",");
	float x = stof(trim(interior.substr(0, pos)));
	interior = interior.substr(pos + 1, interior.length() - pos);
	pos = interior.find(",");
	float y = stof(trim(interior.substr(0, pos)));
	interior = interior.substr(pos + 1, interior.length() - pos);
	pos = interior.find(",");
	float z = stof(trim(interior.substr(0, pos)));

	return glm::vec3(x, y, z);
}
