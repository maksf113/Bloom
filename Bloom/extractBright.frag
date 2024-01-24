#version 430 core

layout (location = 0) out vec4 BrightColor;
in vec2 texCoords;

uniform sampler2D primaryTexture;
uniform int width;
uniform int height;

const float offsetX = 1.0 / float(width);
const float offsetY = 1.0 / float(height);

vec2 offsets[9] = vec2[]
(
	vec2(-offsetX, offsetY), vec2(0.0, offsetY), vec2(offsetX, offsetY),
	vec2(-offsetX, 0.0), vec2(0.0, 0.0), vec2(offsetX, 0.0),
	vec2(-offsetX, -offsetY), vec2(0.0, -offsetY), vec2(offsetX, -offsetY)
);
/*
float kernel[9] = float[]
(
	1,  1,  1,
	1, -8,  1,
	1,  1,  1
);
*/

float kernel[9] = float[]
(
	0,  0,  0,
	0,  1,  0,
	0,  0,  0
);

void main()
{
	/*
	vec3 color = vec3(0.0);
	for(int i = 0; i < 9; i++)
		color+=vec3(texture(screenTexture, texCoords.st + offsets[i])) * kernel[i];
	FragColor = vec4(color, 1.0);
	*/
	vec4 hdrColor = texture(primaryTexture, texCoords);

	float brightness = dot(hdrColor.rgb, hdrColor.rgb);
	brightness = sqrt(brightness);
	//BrightColor = vec4(hdrColor.rgb * (brightness) * 1.0, 1.0);

	if(brightness > 0.5)
		BrightColor = vec4(hdrColor.rgb * pow((brightness - 0.5), 1.0), 1.0);
	else
		BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
	
}