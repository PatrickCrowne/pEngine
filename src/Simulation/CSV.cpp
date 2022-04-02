#include "CSV.h"
#include <fstream>
#include <filesystem>
#include <iostream>
#include "../util.hpp"

CSV::CSV(std::string path)
{

	std::ifstream fileStream;
	fileStream.open(path);

	std::string buffer;
	// Read all the lines of the material configuration file
	int index = -1;
	while (getline(fileStream, buffer)) {
		index++;
		if (index == 0) {
			continue;
		}

		int a = buffer.find_first_of("\t");
		std::cout << buffer.substr(0, a) << "|\n";
		float x = stof(buffer.substr(0, a));

		int b = buffer.find_first_of("\t", a + 1);
		std::cout << buffer.substr(a + 1, b) << "|\n";
		float z = stof(buffer.substr(a + 1, b));

		a = b;
		b = buffer.find_first_of("\t", a + 1);
		std::cout << buffer.substr(a + 1, b) << "|\n";
		float y = stof(buffer.substr(a + 1, b));

		positions.push_back(glm::vec3(x, y, z));

	}
}
