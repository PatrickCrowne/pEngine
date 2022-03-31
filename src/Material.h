#pragma once
#include <vector>
#include <string>
#include "Shader.h"

#define MAT_MATERIAL_NAME "MATERIAL_NAME"
#define MAT_VERTEX_SHADER "VERTEX_SHADER"
#define MAT_FRAGMENT_SHADER "FRAGMENT_SHADER"
#define MAT_PROPERTY "PROPERTY"
#define MAT_MODEL_MATRIX "MODEL_MATRIX"

class Material
{
public:
	Material(std::string);
	std::string name;
	std::string modelMatrixAttributeName;
	std::vector<std::string> attributes;
	Shader *shader;
private:
	std::string getBetweenQuotes(std::string);
	void getPropertyNameAndValue(std::string, std::string*, std::string *);
};

