#include "Material.h"
#include <iostream>
#include <fstream>
#include <filesystem>

// Creates a new material from the configuration file
Material::Material(std::string materialConfigFile) {

	std::ifstream materailConfigFileStream;
	materailConfigFileStream.open(materialConfigFile);

	std::string buffer;
	std::string vertexShaderFile;
	std::string fragmentShaderFile;
	// Read all the lines of the material configuration file
	while (getline(materailConfigFileStream, buffer)) {

		// Material Name
		if (buffer._Starts_with(MAT_MATERIAL_NAME)) {
			name = getBetweenQuotes(buffer);
			std::cout << name << "\n";
			continue;
		}
		// Vertex Shader
		if (buffer._Starts_with(MAT_VERTEX_SHADER)) {
			vertexShaderFile = getBetweenQuotes(buffer);
			continue;
		}
		// Fragment Shader
		if (buffer._Starts_with(MAT_FRAGMENT_SHADER)) {
			fragmentShaderFile = getBetweenQuotes(buffer);
			continue;
		}

		// Properties
		if (buffer._Starts_with(MAT_PROPERTY)) {

			std::string propertyName;
			std::string propertyValue;
			getPropertyNameAndValue(buffer, &propertyName, &propertyValue);

			// Special cases
			// Model Matrix identifier
			if (propertyValue._Equal(MAT_MODEL_MATRIX)) {
				modelMatrixAttributeName = propertyName;
			}

		}

	}

	materailConfigFileStream.close();

	if (vertexShaderFile.empty() || fragmentShaderFile.empty()) {
		std::cout << "Error loading material from file " << materialConfigFile << "!\n";
	}
	else {
		// Create the shader
		shader = new Shader(vertexShaderFile, fragmentShaderFile);
	}
	

}

/// <summary>
/// Returns the information between the quotes in the given line
/// </summary>
/// <param name="line"></param>
/// <returns></returns>
std::string Material::getBetweenQuotes(std::string line) {
	int zero = line.find("\"") + 1;
	return line.substr(zero, line.find_last_of("\"") - zero);
}


/// <summary>
/// Returns the name and value of a property given the line
/// </summary>
/// <param name="line">The line of code containing the info</param>
/// <param name="name_ref">The reference to the string storing the name</param>
/// <param name="value_ref">The reference to the string storing the value</param>
void Material::getPropertyNameAndValue(std::string line , std::string *name_ref, std::string * value_ref) {

	// Get rid of the "PROPERTY" keyword
	line = line.substr(line.find(" ") + 1, line.length());
	// Split between the equals
	*name_ref = line.substr(0, line.find("="));
	*value_ref = line.substr(line.find("=") + 1, line.length());
	// Sanitize
	*name_ref = (*name_ref).substr((*name_ref).find_first_not_of(" "), (*name_ref).find_last_of(" "));
	*value_ref = (*value_ref).substr((*value_ref).find_first_not_of(" "), (*value_ref).length());
	std::cout << (*name_ref) << " -> " << (*value_ref) << " \n";

}