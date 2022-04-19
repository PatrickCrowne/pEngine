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

vec3 sunPos = vec3(0,1,0);

vec3 reflect(vec3 ray, vec3 normal) {

	vec3 horiz = ray - (dot(ray, normal) * normal);
	return normalize(ray - (2 * horiz));

}

void main() {

	vec3 col = fragColor;

	// Shade based on light
		col = mix(fragColor * 0.25f, fragColor, clamp(dot(fragNorm, sunPos), 0, 1));
		vec3 camOffset = normalize(camPos - fragPos);
		vec3 reflected = reflect(normalize(camOffset), normalize(fragNorm));

		

	// Blend Reflection
		float r = sqrt(reflected.x*reflected.x+reflected.y*reflected.y+reflected.z*reflected.z);
		float theta = atan(reflected.y,reflected.x) / r;
		float phi = atan(sqrt(reflected.x*reflected.x+reflected.y*reflected.y),reflected.z) / r;
		vec3 reflectedCol = texture(reflectionTexture, vec2(theta / 180 * 3.14159, phi / 180 * 3.14159)).xyz;
		float fresnel = 1 - clamp(dot(-normalize(camOffset), normalize(fragNorm)), 0, 1);
		col = mix(col, reflectedCol, fresnel * reflectionStrength);

	// Specular Highlight
		vec3 specularCol = pow(clamp(dot(sunPos, reflected), 0, 1), 8) * vec3(1,1,1);
		col += specularCol;

	outCol = col;
	
}
