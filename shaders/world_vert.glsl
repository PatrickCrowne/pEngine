#version 330

layout(location = 0) in vec3 pos;		// Model-space position
layout(location = 1) in vec3 norm;		// Model-space normal
layout(location = 2) in vec2 uv;		// Model-space uv

smooth out vec3 fragNorm;	// Model-space interpolated normal
smooth out vec2 fragUV;

uniform mat4 viewProjMatrix;
uniform mat4 modelMatrix;

void main() {
	// Transform vertex position
	gl_Position = viewProjMatrix * vec4(pos, 1.0);
	fragUV = pos.xz / 3;
	fragNorm = norm;
}
