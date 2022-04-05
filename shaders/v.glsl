#version 330

layout(location = 0) in vec3 pos;		// Model-space position
layout(location = 1) in vec3 norm;		// Model-space normal
layout(location = 2) in vec2 uv;		// Model-space uv

smooth out vec3 fragNorm;	// Model-space interpolated normal
smooth out vec3 fragColor;
smooth out vec3 fragPos;
smooth out vec2 fragUV;
smooth out vec3 camPos;
smooth out vec3 modelPos;

uniform mat4 xform;
uniform vec3 color;
uniform vec3 camera;
uniform vec3 model;

void main() {
	// Transform vertex position
	gl_Position = xform * vec4(pos, 1.0);
	fragPos = pos;
	fragUV = uv;

	// Interpolate normals
	fragNorm = norm;
	fragColor = color;
	camPos = camera;
	modelPos = model; 
}
