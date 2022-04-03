#version 330

smooth in vec3 fragNorm;	// Interpolated model-space normal
smooth in vec3 fragColor;
smooth in vec3 fragPos;
smooth in vec2 fragUV;

uniform sampler2D sampleTexture;

out vec3 outCol;	// Final pixel color

void main() {
	// Visualize normals as colors
	vec3 col = texture(sampleTexture, fragUV).xyz;
	col *= dot(fragNorm, vec3(1,1,1));
	col += vec3(35 / 255, 35 / 255, 55 / 255);
	outCol = col;
}
