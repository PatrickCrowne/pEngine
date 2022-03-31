#pragma once
#include "gl_core_3_3.h"
#include <string>
#include <map>

#define SHADER_INPUT_TYPE_1F	0	// For single variable floating point inputs
#define SHADER_INPUT_TYPE_3FV	1	// For triple variable floating point inputs (vectors)
#define SHADER_INPUT_TYPE_4FV	2	// For matrix inputs
#define SHADER_INPUT_TYPE_1I	3	// For single variable integer inputs

class Shader
{
public:
	Shader();
	Shader(std::string vertexFile, std::string fragmentFile);
	GLuint compiledShaderId;
	std::map<std::string, GLuint> uniformInputs;
	std::map<std::string, GLuint> uniformInputType;
private:
	int determineDatatype(std::string);
};

