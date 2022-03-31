#pragma once
#include <map>
#include <string>
#include "Shader.h"
#include <glm/glm.hpp>

#define MAT_MATERIAL_NAME "MATERIAL_NAME"
#define MAT_VERTEX_SHADER "VERTEX_SHADER"
#define MAT_FRAGMENT_SHADER "FRAGMENT_SHADER"
#define MAT_PROPERTY "PROPERTY"
#define MAT_MODEL_MATRIX "MODEL_MATRIX"

#define MAT_PROP_VECTOR3 "vec3"

class Material
{
public:
	Material(std::string);
	std::string name;
	std::string modelMatrixAttributeName;
	bool registerAttribute(std::string name, int type, void* value);
	int getAttributeValue(std::string in, void* out);
	void applyAttributes();
	Shader *shader;
private:
	std::string getBetweenQuotes(std::string);
	void getPropertyNameAndValue(std::string, std::string*, std::string *);
	
	// Attributes
	std::map <std::string, int> integerAttributes;
	std::map <std::string, float> floatAttributes;
	std::map <std::string, glm::vec3> vectorAttributes;
	std::map <std::string, glm::mat4> matrixAttributes;

};
