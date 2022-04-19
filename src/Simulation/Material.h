#pragma once
#include <map>
#include <string>
#include <vector>
#include "Shader.h"
#include <glm/glm.hpp>
#include "Texture.h"

#define MAT_MATERIAL_NAME "MATERIAL_NAME"
#define MAT_VERTEX_SHADER "VERTEX_SHADER"
#define MAT_FRAGMENT_SHADER "FRAGMENT_SHADER"
#define MAT_PROPERTY "PROPERTY"
#define MAT_MODEL_MATRIX "MODEL_MATRIX"
#define MAT_VIEWPROJ_MATRIX "VIEWPROJ_MATRIX"
#define MAT_CAMERA_POSITION "CAMREA_POSITION"
#define MAT_MODEL_POSITION "MODEL_POSITION"

#define MAT_PROP_VECTOR3 "vec3"
#define MAT_PROP_INTEGER "int"
#define MAT_PROP_FLOAT "float"
#define MAT_PROP_TEXTURE2D "tex"

class Material
{
public:
	Material(std::string);
	std::string name;
	std::string modelMatrixAttributeName;
	std::string viewProjMatrixAttributeName;
	std::string cameraPositionAttributeName;
	std::string modelPositionAttributeName;
	bool registerAttribute(std::string name, int type, void* value);
	int getAttributeValue(std::string in, void* out);
	void applyAttributes();
	Shader *shader;
	static Material *getMaterial(std::string);
private:
	void getPropertyNameAndValue(std::string, std::string*, std::string *);
	std::vector<Texture*> textures;
	// Attributes
	std::map <std::string, int> integerAttributes;
	std::map <std::string, float> floatAttributes;
	std::map <std::string, glm::vec3> vectorAttributes;
	std::map <std::string, glm::mat4> matrixAttributes;
	int materialId;
	static std::map<std::string, Material*> materials;

};

