#version 330

smooth in vec3 fragNorm;	// Interpolated model-space normal
smooth in vec3 fragColor;
smooth in vec3 fragPos;
smooth in vec2 fragUV;
smooth in vec3 camPos;
smooth in vec3 modelPos;

smooth in vec4 lightFragPos;    // Fragment position in light space

uniform sampler2D sampleTexture;
uniform sampler2D reflectionTexture;
uniform sampler2D shadowMap;
uniform float reflectionStrength;

out vec3 outCol;	// Final pixel color

vec3 sunPos = vec3(1,1,1);

vec3 reflect(vec3 ray, vec3 normal) {

	vec3 horiz = ray - (dot(ray, normal) * normal);
	return normalize(ray - (2 * horiz));

}

float calculateShadow(vec4 light_frag_pos) {  // TODO: add other parameters if needed
	// Perspective divide
	vec3 projCoords = light_frag_pos.xyz / light_frag_pos.w;
	// Remap to [0.0, 1.0]
	projCoords = projCoords * 0.5 + 0.5;

	// TODO: =========================================================
	// Decide whether a fragment is in shadow
	// Read the depth from depth map, then compare it with the depth of the current fragment

	float shadowDepth = texture(shadowMap, projCoords.xy).r;
	float fragDepth = projCoords.z;
	
	if (shadowDepth < fragDepth) return 1;
	return 0;

}

void main() {

	vec2 uvRail = vec2(fragUV.x, fragUV.y);
	vec3 col = texture(sampleTexture, uvRail).xyz * fragColor;

	// Shade based on light
		col = mix(col * 0.25f, col, clamp(dot(fragNorm, sunPos), 0, 1));
		vec3 camOffset = normalize(fragPos - camPos);
		vec3 reflected = reflect(normalize(camOffset), normalize(fragNorm));

		

	// Blend Reflection

		vec2 uv;
		// Computing longitude
		uv.x = atan( -reflected.z, -reflected.x ) * (1/3.1415f) + 0.5;
		// Computing latitude
		uv.y = reflected.y * 0.5 + 0.5;

		vec3 reflectedCol = vec3(0,0,0);
		reflectedCol  += texture(reflectionTexture, uv).xyz;

		float fresnel = 1 - clamp(dot(-normalize(camOffset), normalize(fragNorm)), 0, 1);
		col = mix(col, reflectedCol, fresnel * reflectionStrength + 0.1f);

	// Specular Highlight
		vec3 specularCol = pow(clamp(dot(-1 * sunPos, reflected), 0, 1), 8) * vec3(1,1,1);
		col += specularCol * 0.5f;

	// Shadows from sun
	float shadow = calculateShadow(lightFragPos);

	outCol = col;// * (1 - shadow);
	
}
