#include "Material.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include "../util.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "AssetReader.h"

std::map<std::string, Material*> Material::materials;

// Creates a new material from the configuration file
Material::Material(std::string materialConfigFile) {

	std::ifstream materailConfigFileStream;
	materailConfigFileStream.open(materialConfigFile);

	std::string buffer;
	std::string shaderName;
	std::string vertexShaderFile;
	std::string fragmentShaderFile;
	// Read all the lines of the material configuration file
	while (getline(materailConfigFileStream, buffer)) {

		// Material Name
		if (buffer._Starts_with(MAT_MATERIAL_NAME)) {
			name = AssetReader::getBetweenCharacters(buffer, "\"");
			continue;
		}
		// Vertex Shader
		if (buffer._Starts_with(MAT_VERTEX_SHADER)) {
			vertexShaderFile = AssetReader::getBetweenCharacters(buffer, "\"");
			continue;
		}
		// Fragment Shader
		if (buffer._Starts_with(MAT_FRAGMENT_SHADER)) {
			fragmentShaderFile = AssetReader::getBetweenCharacters(buffer, "\"");
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
			// View Projection Matrix Identifier
			if (propertyValue._Equal(MAT_VIEWPROJ_MATRIX)) {
				viewProjMatrixAttributeName = propertyName;
			}
			else if (propertyValue._Equal(MAT_LIGHT_MATRIX)) {
				lightMatrixAttributeName = propertyName;
			}
			// Camrea Pos identifier
			else if (propertyValue._Equal(MAT_CAMERA_POSITION)) {
				cameraPositionAttributeName = propertyName;
			}
			// Camrea Pos identifier
			else if (propertyValue._Equal(MAT_MODEL_POSITION)) {
				modelPositionAttributeName = propertyName;
			}
			// Process generic attributes
			else {

				// Integer
				if (propertyValue._Starts_with(MAT_PROP_INTEGER)) {
					int value = AssetReader::getInteger(propertyValue);
					registerAttribute(propertyName, SHADER_INPUT_TYPE_1I, (void*)&value);
				}

				// Float
				if (propertyValue._Starts_with(MAT_PROP_FLOAT)) {
					float value = AssetReader::getFloat(propertyValue);
					registerAttribute(propertyName, SHADER_INPUT_TYPE_1F, (void*) &value);
				}

				// Vector
				if (propertyValue._Starts_with(MAT_PROP_VECTOR3)) {
					glm::vec3 value = AssetReader::getVector(propertyValue);
					registerAttribute(propertyName, SHADER_INPUT_TYPE_3FV, (void*)&value);
				}

				// Texture2D
				if (propertyValue._Starts_with(MAT_PROP_TEXTURE2D)) {

					Texture *texture = new Texture(AssetReader::getTexture(propertyValue));
					int value = textures.size();
					registerAttribute(propertyName, SHADER_INPUT_TYPE_SAMPLER2D_1I, (void*)&value);
					textures.push_back(texture);

				}

			}
			

		}

	}

	materailConfigFileStream.close();

	if (vertexShaderFile.empty() || fragmentShaderFile.empty()) {
		std::cout << "Error loading material from file " << materialConfigFile << "!\n";
		shader = nullptr;
	}
	else {
		// Create the shader
		shader = new Shader(vertexShaderFile, fragmentShaderFile);
	}
	

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
	if (type == SHADER_INPUT_TYPE_SAMPLER2D_1I) {
		integerAttributes.emplace(name, *((int*)value));
	}
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
	return SHADER_INPUT_TYPE_1F;

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
		glUniform1i(shader->uniformInputs.at(intAttributeIterator->first), intAttributeIterator->second);
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

	

	int index = 0;
	for (Texture *t : textures) {
		t->activeTexture(index);
		index++;
	}

}

Material *Material::getMaterial(std::string path)
{
	if (materials.count(path) > 0) {
		return materials.at(path);
	}
	return new Material(path);
}
