#version 330

smooth in vec3 fragNorm;	// Interpolated model-space normal
smooth in vec3 fragColor;
smooth in vec3 fragPos;
smooth in vec2 fragUV;
smooth in vec3 camPos;
smooth in vec3 modelPos;

uniform sampler2D sampleTexture;
uniform sampler2D reflectionTexture;
uniform float reflectionStrength;

out vec3 outCol;	// Final pixel color

vec3 sunPos = vec3(1,1,1);

void main() {
	// Visualize normals as colors
	vec3 col = fragColor;
	// Shade based on light
	col *= clamp(dot(fragNorm, sunPos), 0, 1);
	// Ambient color
	col.xyz += vec3(1, 1, 1) * 0.1f;

	vec3 camOffset = normalize(camPos - (fragPos + modelPos));

	// Blend Reflection
	vec3 reflected = (camOffset * -1) + (dot(fragNorm, camOffset) * 2 * fragNorm);
	
	float r = sqrt(reflected.x*reflected.x+reflected.y*reflected.y+reflected.z*reflected.z);
	float theta = atan(reflected.y,reflected.x) / r;
	float phi = atan(sqrt(reflected.x*reflected.x+reflected.y*reflected.y),reflected.z) / r;

	vec3 reflectedCol = texture(reflectionTexture, vec2(theta / 180 * 3.14159, phi / 180 * 3.14159)).xyz;

	float fresnel = 1 - clamp(dot(-normalize(camOffset), normalize(fragNorm)), 0, 1);

	col = mix(col, reflectedCol, fresnel * reflectionStrength);

	outCol = col;
}
