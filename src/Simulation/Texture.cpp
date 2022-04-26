#include <iostream>
#include <cassert>
#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image.h"

std::map<std::string, GLuint> Texture::textures;

Texture::Texture(std::string path) {
	// If this texture already exists, don't make it again
	if (textures.count(path) > 0) {
		id = textures.at(path);
	}
	else {
		id = prepareTexture(path.c_str());
		
		textures.emplace(path, id);
	}
	
}

Texture::Texture() {
	id = prepareDepthMap();
	textures.emplace("DEPTH", id);
}

void Texture::activeTexture(int textureIndex) {
	// Activate the texture unit; more than one texture is allowed to use: GL_TEXTURE0, GL_TEXTURE1, ...
	if (textureIndex == 0) glActiveTexture(GL_TEXTURE0);
	if (textureIndex == 1) glActiveTexture(GL_TEXTURE1);
	if (textureIndex == 2) glActiveTexture(GL_TEXTURE2);
	if (textureIndex == 3) glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, id);  // bind the texture
	glEnable(GL_TEXTURE_2D);
}

unsigned int Texture::prepareTexture(const char* filename) {
	GLuint texture;
	int image_height = 0, image_width = 0, num_channels = 0;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// Sets wrapping and filtering of the texture.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// the height&width of the image must be multiples of 4
	unsigned char* image_data = stbi_load(filename, &image_width, &image_height, &num_channels, 0);

	// Do some simple checking
	if (image_data == nullptr) {
		std::cerr << "Image reading failed: " << filename << std::endl;
		return -1;
	}
	else if (num_channels != 3 && num_channels != 4) {
		std::cerr << "The loaded image doesn't have RGB color components." << std::endl;
		std::cerr << "The loaded image has " << num_channels << " channels" << std::endl;
		return -1;
	}
	else {
#ifdef DEBUG
		std::cout << "The image " << filename << " loaded has size " << image_width << "x" << image_height << std::endl;
#endif // DEBUG
	}
	glTexImage2D(GL_TEXTURE_2D,		// The texture is 2D. (guess what, 1D and 3D are also supported by OpenGL.)
		0,							// level of mipmapping. Just make this 0.
		GL_RGB,						// How the image should be represented.
		image_width, image_height,	// width and height of the image
		0,							// Don't worry about this.
		GL_RGB,						// The format of the image data source. `image_data` has 3 channels.
		GL_UNSIGNED_BYTE,			// Data type of the image data source. `image_data` is an array of unsigned char.
		image_data
	);
	// When MAGnifying the image (no bigger mipmap available), use LINEAR filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// When MINifying the image, use a LINEAR blend of two mipmaps, each filtered LINEARLY too
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	// Generates mipmapping for better sampling.
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(image_data);
	assert(glGetError() == GL_NO_ERROR);

	return texture;
}

unsigned int Texture::prepareDepthMap() {

	GLuint depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);  // Generate a frame buffer

	glGenTextures(1, &depth);
	glBindTexture(GL_TEXTURE_2D, depth);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 2048, 2048, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Attach depth texture to the depth frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return depthMapFBO;

}