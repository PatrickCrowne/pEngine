#pragma once
#include <string>
#include <map>
#include "../gl_core_3_3.h"

class Texture {
public:
	Texture(std::string);
	Texture();
	~Texture() {}
	// Disallow copy, move, & assignment
	/*
	Texture(const Texture& other) = delete;
	Texture& operator=(const Texture& other) = delete;
	Texture(Texture&& other) = delete;
	Texture& operator=(Texture&& other) = delete;
	*/
	void activeTexture(int textureIndex);

	static std::map<std::string, GLuint> textures;

	GLuint id;     // Texture id
	GLuint depth;

protected:
	
	unsigned int prepareTexture(const char* filename);
	unsigned int prepareDepthMap();
};

