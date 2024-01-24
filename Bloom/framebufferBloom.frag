#version 430 core

out vec4 FragColor;

in vec2 texCoords;

uniform sampler2D primaryTexture;
uniform sampler2D bloomBlur;

void main()
{
	vec3 hdrColor = texture(primaryTexture, texCoords).rgb;
	vec3 bloomColor = texture(bloomBlur, texCoords).rgb;

	vec3 sum = hdrColor + bloomColor;

	float exposure = 0.3;
	vec3 toneMapped = vec3(1.0) - exp(-sum * exposure);

	FragColor = vec4(toneMapped, 1.0);
}