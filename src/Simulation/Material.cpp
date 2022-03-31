#include "Material.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include "../util.hpp"
#include <glm/gtc/type_ptr.hpp>

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
			// Process generic attributes
			else {
				// Colors
				if (propertyValue._Starts_with(MAT_PROP_VECTOR3)) {
					propertyValue = propertyValue.substr(5, propertyValue.length() - 6);
					size_t pos = propertyValue.find(",");

					float x = stof(trim(propertyValue.substr(0, pos)));
					propertyValue = propertyValue.substr(pos + 1, propertyValue.length() - pos);
					pos = propertyValue.find(",");

					float y = stof(trim(propertyValue.substr(0, pos)));
					propertyValue = propertyValue.substr(pos + 1, propertyValue.length() - pos);
					pos = propertyValue.find(",");

					float z = stof(trim(propertyValue.substr(0, pos)));

					// Register the vec3
					registerAttribute(propertyName, SHADER_INPUT_TYPE_3FV, (void*) &glm::vec3(x, y, z));
				}
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
	int firstIndex = line.find("\"") + 1; // Stores the index of the first quotation mark
	return line.substr(firstIndex, line.find_last_of("\"") - firstIndex);
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

/// <summary>
/// Registers a material attribute value to this material
/// </summary>
/// <param name="name"></param>
/// <param name="type"></param>
/// <param name="value"></param>
/// <returns></returns>
bool Material::registerAttribute(std::string name, int type, void* value) {

	if (type == SHADER_INPUT_TYPE_1F) floatAttributes.emplace(name, *((float*)value));
	if (type == SHADER_INPUT_TYPE_3FV) vectorAttributes.emplace(name, *((glm::vec3*)value));
	if (type == SHADER_INPUT_TYPE_4FV) matrixAttributes.emplace(name, *((glm::mat4*)value));
	if (type == SHADER_INPUT_TYPE_1I) integerAttributes.emplace(name, *((int*)value));
	return true;

}

/// <summary>
/// Finds the attribute value and returns it's datatype
/// </summary>
/// <param name="in"></param>
/// <param name="out"></param>
/// <returns></returns>
int Material::getAttributeValue(std::string in, void* out) {

	if (floatAttributes.count(in)) {
		out = &floatAttributes.at(in);
		return SHADER_INPUT_TYPE_1F;
	}
	if (integerAttributes.count(in)) {
		out = &integerAttributes.at(in);
		return SHADER_INPUT_TYPE_1I;
	}
	if (vectorAttributes.count(in)) {
		out = &vectorAttributes.at(in);
		return SHADER_INPUT_TYPE_3FV;
	}
	if (matrixAttributes.count(in)) {
		out = &matrixAttributes.at(in);
		return SHADER_INPUT_TYPE_4FV;
	}

}

/// <summary>
/// Applies all the material attributes for this material
/// </summary>
void Material::applyAttributes() {

	// Iterate through all float attributes and apply them
	std::map<std::string, float>::iterator floatAttributeIterator;
	for (floatAttributeIterator = floatAttributes.begin(); floatAttributeIterator != floatAttributes.end(); floatAttributeIterator++)
	{
		glUniform1f(shader->uniformInputs.at(floatAttributeIterator->first), floatAttributeIterator->second);
	}

	// Iterate through all int attributes and apply them
	std::map<std::string, int>::iterator intAttributeIterator;
	for (intAttributeIterator = integerAttributes.begin(); intAttributeIterator != integerAttributes.end(); intAttributeIterator++)
	{
		glUniform1f(shader->uniformInputs.at(intAttributeIterator->first), intAttributeIterator->second);
	}

	// Iterate through all vec3 attributes and apply them
	std::map<std::string, glm::vec3>::iterator vec3AttributeIterator;
	for (vec3AttributeIterator = vectorAttributes.begin(); vec3AttributeIterator != vectorAttributes.end(); vec3AttributeIterator++)
	{
		glUniform3fv(shader->uniformInputs.at(vec3AttributeIterator->first), 1, glm::value_ptr(vec3AttributeIterator->second));
	}

	// Iterate through all mat4 attributes and apply them
	std::map<std::string, glm::mat4>::iterator mat4AttributeIterator;
	for (mat4AttributeIterator = matrixAttributes.begin(); mat4AttributeIterator != matrixAttributes.end(); mat4AttributeIterator++)
	{
		glUniformMatrix4fv(shader->uniformInputs.at(mat4AttributeIterator->first), 1, GL_FALSE, glm::value_ptr(mat4AttributeIterator->second));
	}

}