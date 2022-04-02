#include "Shader.h"
#include <vector>
#include "../util.hpp"
#include <iostream>
#include <fstream>
#include <string>

/// <summary>
/// Constructs a new shader
/// </summary>
/// <param name="vertexFile">The path to the vertex shader file</param>
/// <param name="fragmentFile">The path to the fragment shader file</param>
Shader::Shader(std::string vertexFile, std::string fragmentFile) {

	// Compile and link shader files
	std::vector<GLuint> shaders;
	shaders.push_back(compileShader(GL_VERTEX_SHADER, vertexFile));
	shaders.push_back(compileShader(GL_FRAGMENT_SHADER, fragmentFile));
	compiledShaderId = linkProgram(shaders);
	// Cleanup extra state
	for (auto s : shaders)
		glDeleteShader(s);
	shaders.clear();

	std::cout << "Shader: " << vertexFile << "\n";

	// Get uniform identifers
	std::ifstream vertexFileSteam;
	vertexFileSteam.open(vertexFile);
	std::string buffer;
	// Read all the uniform identifers from the file
	while (getline(vertexFileSteam, buffer)) {
		if (buffer._Starts_with("uniform")) {
			size_t pos;
			// Get rid of any comments that may be on this line
			pos = buffer.find("//");
			if (pos != std::string::npos) {
				buffer = buffer.substr(0, pos);
			}
			// Get the uniform identifier
			// Skip to the space after 'uniform'
			pos = buffer.find(' ');
			buffer = buffer.substr(pos + 1, buffer.length());
			// Skip to the space after the type
			pos = buffer.find(' ');
			// Get the datatype
			std::string dataString = buffer.substr(0, pos);
			// Determine data type
			int dataType = determineDatatype(dataString);
			buffer = buffer.substr(pos + 1, buffer.length());
			// Sanitize the end
			pos = buffer.find(';');
			if (pos != std::string::npos) {
				buffer = buffer.substr(0, pos);
			}
			// Add the uniform input to the map
			uniformInputs.emplace(buffer, glGetUniformLocation(compiledShaderId, buffer.c_str()));
			uniformInputType.emplace(buffer, dataType);
		}
	}

	vertexFileSteam.close();


	// Get uniform identifers
	std::ifstream fragmentFileStream;
	fragmentFileStream.open(fragmentFile);
	// Read all the uniform identifers from the file
	while (getline(fragmentFileStream, buffer)) {
		if (buffer._Starts_with("uniform")) {
			size_t pos;
			// Get rid of any comments that may be on this line
			pos = buffer.find("//");
			if (pos != std::string::npos) {
				buffer = buffer.substr(0, pos);
			}
			// Get the uniform identifier
			// Skip to the space after 'uniform'
			pos = buffer.find(' ');
			buffer = buffer.substr(pos + 1, buffer.length());
			// Skip to the space after the type
			pos = buffer.find(' ');
			// Get the datatype
			std::string dataString = buffer.substr(0, pos);
			// Determine data type
			int dataType = determineDatatype(dataString);
			buffer = buffer.substr(pos + 1, buffer.length());
			// Sanitize the end
			pos = buffer.find(';');
			if (pos != std::string::npos) {
				buffer = buffer.substr(0, pos);
			}
			// Add the uniform input to the map
			uniformInputs.emplace(buffer, glGetUniformLocation(compiledShaderId, buffer.c_str()));
			uniformInputType.emplace(buffer, dataType);
		}
	}

	fragmentFileStream.close();

}

/// <summary>
/// Destroys this shader
/// </summary>
Shader::~Shader()
{
	glDeleteProgram(compiledShaderId);
}

/// <summary>
/// Determines the data type for the given string input
/// </summary>
/// <param name="">The string input of the datatype "vec"</param>
/// <returns></returns>
int Shader::determineDatatype(std::string dataString) {

	if (dataString._Equal("mat4")) {
		return SHADER_INPUT_TYPE_4FV;
	}

	if (dataString._Equal("vec3")) {
		return SHADER_INPUT_TYPE_3FV;
	}

	if (dataString._Equal("int")) {
		return SHADER_INPUT_TYPE_1I;
	}

	if (dataString._Equal("sampler2D")) {
		return SHADER_INPUT_TYPE_SAMPLER2D_1I;
	}

	return SHADER_INPUT_TYPE_1F;

}
