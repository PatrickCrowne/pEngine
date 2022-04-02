#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>
class CSV
{

public:
	CSV(std::string path);
	std::vector<glm::vec3> positions;

};

