#version 430 core

out vec4 FragColor;

in vec2 texCoords;

uniform sampler2D texToBlur;

uniform bool horizontal;
uniform bool odd;
// uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

uniform float weight[7] = float[] (0.19764065288095692,
								   0.17486827308986308,
								   0.12111723251079277,
								   0.06566651775036979,
								   0.027866844247689634,
								   0.009255297393309877,
								   0.002405508567496413);

void main()
{
	vec2 texOffset = 1.0 / textureSize(texToBlur, 0);
	vec3 result = texture(texToBlur, texCoords).rgb * weight[0]; // crnt frag
	
	float factor = odd ? 1.0 : 2.0;

	if(horizontal)
		for(int i = 1; i < 7; i++)
		{
			result += texture(texToBlur, texCoords + vec2(texOffset.y * i, 0.0)).rgb * weight[i];
			result += texture(texToBlur, texCoords - vec2(texOffset.y * i, 0.0)).rgb * weight[i];
		}											 
	else											 
		for(int i = 1; i < 7; i++)					 
		{											 
			result += texture(texToBlur, texCoords + vec2(0.0, texOffset.y * i)).rgb * weight[i];
			result += texture(texToBlur, texCoords - vec2(0.0, texOffset.y * i)).rgb * weight[i];
		}
	FragColor = vec4(result, 1.0);
}