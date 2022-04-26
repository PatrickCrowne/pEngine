#version 330

layout(location = 0) in vec3 pos;		// Model-space position
layout(location = 1) in vec3 norm;		// Model-space normal
layout(location = 2) in vec2 uv;		// Model-space uv

smooth out vec3 fragNorm;	// Model-space interpolated normal
smooth out vec3 fragColor;
smooth out vec3 fragPos;
smooth out vec2 fragUV;
smooth out vec3 camPos;
smooth out vec4 lightFragPos;   // Fragment position in light space

uniform mat4 viewProjMatrix;
uniform mat4 modelMatrix;
uniform vec3 color;
uniform vec3 camera;
uniform mat4 lightSpaceMat;  // World-to-light matrix location

void main() {
	// Transform vertex position
	
	fragPos = vec3(modelMatrix * vec4(pos, 1.0));
	fragNorm = vec3(modelMatrix * vec4(norm, 0.0));
	fragUV = uv;
	fragColor = color;
	camPos = -camera;	// Camera position is reversed, to make it relative

	lightFragPos = lightSpaceMat * vec4(fragPos, 1.0);

	gl_Position = viewProjMatrix * vec4(fragPos, 1.0);

}
