#version 330

smooth in vec3 fragNorm;	// Interpolated model-space normal
smooth in vec3 fragColor;

out vec3 outCol;	// Final pixel color

void main() {
	// Visualize normals as colors
	outCol = normalize(fragNorm) * 0.5f + vec3(0.5f);
}
