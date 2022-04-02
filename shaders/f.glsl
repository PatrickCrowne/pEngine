#version 330

smooth in vec3 fragNorm;	// Interpolated model-space normal
smooth in vec3 fragColor;
smooth in vec3 fragPos;
smooth in vec2 fragUV;

uniform sampler2D sampleTexture;

out vec3 outCol;	// Final pixel color

void main() {
	// Visualize normals as colors
	outCol.rgb = texture(sampleTexture, fragUV).xyz;
}
