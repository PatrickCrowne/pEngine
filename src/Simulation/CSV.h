#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>
class CSV
{

public:
	CSV(std::string path);
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> forward;
	std::vector<glm::vec3> left;
	std::vector<glm::vec3> up;

};

