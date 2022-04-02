#version 330

layout(location = 0) in vec3 pos;		// Model-space position
layout(location = 1) in vec3 norm;		// Model-space normal
layout(location = 2) in vec2 uv;		// Model-space uv

smooth out vec3 fragNorm;	// Model-space interpolated normal
smooth out vec3 fragColor;
smooth out vec3 fragPos;
smooth out vec2 fragUV;

uniform mat4 xform;			// Model-to-clip space transform
uniform vec3 color;
uniform vec3 colorB;
uniform int a;
uniform float b;

void main() {
	// Transform vertex position
	gl_Position = xform * vec4(pos, 1.0);
	fragPos = pos;
	fragUV = uv;

	// Interpolate normals
	fragNorm = norm;
	fragColor = mix(color, colorB, dot(norm, vec3(0, 1, 0)));
}
