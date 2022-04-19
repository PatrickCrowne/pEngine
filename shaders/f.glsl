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

	vec2 uvRail = vec2(fragUV.x, fragUV.y);
	vec3 col = texture(sampleTexture, uvRail).xyz * fragColor;

	// Shade based on light
		col = mix(col * 0.25f, col, clamp(dot(fragNorm, sunPos), 0, 1));
		vec3 camOffset = normalize(camPos - fragPos);
		vec3 reflected = reflect(normalize(camOffset), normalize(fragNorm));

		

	// Blend Reflection

		vec2 uv;
		// Computing longitude
		uv.x = atan( -reflected.z, -reflected.x ) * (1/3.1415f) + 0.5;
		// Computing latitude
		uv.y = reflected.y * 0.5 + 0.5;

		vec3 reflectedCol = texture(reflectionTexture, uv).xyz;
		float fresnel = 1 - clamp(dot(-normalize(camOffset), normalize(fragNorm)), 0, 1);
		col = mix(col, reflectedCol, fresnel * reflectionStrength + 0.1f);

	// Specular Highlight
		vec3 specularCol = pow(clamp(dot(sunPos, reflected), 0, 1), 8) * vec3(1,1,1);
		col += specularCol * 0.5f;

	outCol = col;
	
}
