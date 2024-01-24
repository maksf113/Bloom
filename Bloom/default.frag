#version 430 core
#define MAX_POINT_LIGHT_NR 3

out vec4 FragColor;

in vec3 crntPos;
in vec3 Normal;
in vec2 texCoord;

in vec4 posDirLightVP;

uniform sampler2D diffuse0;
uniform sampler2D specular0;
uniform sampler2D shadowMap;

uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;
uniform vec3 camDir;
uniform bool flashLight;

struct PointLight 
{    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

	vec3 ambient;
    vec3 diffuse;
    vec3 specular;

	samplerCube shadowMap;
}; 

uniform PointLight pointLights[MAX_POINT_LIGHT_NR];;

uniform float pointLightFarPlane; // for shadows


float CalcPointShadow(PointLight light, vec3 fragPos)
{
	float shadow = 0.0;
	vec3 fragFromLight = fragPos - light.position;
	float closestDepth;
	closestDepth = texture(light.shadowMap, fragFromLight).r;
	closestDepth *= pointLightFarPlane;
	float currentDepth = length(fragFromLight);
	float bias = max(0.5 * (1.0 - dot(Normal, normalize(-fragFromLight))), 0.0005);
	if(currentDepth > closestDepth + bias)
		shadow = 1.0;
	/*
	int sampleRadius = 1;
	float pixelSize = 1.0 / 1024.0;
	
	float maxComponent = abs(fragFromLight.x);
	vec3 samplingVec1 = vec3(0.0, 1.0, 0.0);
	vec3 samplingVec2 = vec3(0.0, 0.0, 1.0);

	if(maxComponent < abs(fragFromLight.y))
	{
		maxComponent = abs(fragFromLight.y);
		samplingVec1 = vec3(1.0, 0.0, 0.0);
		samplingVec2 = vec3(0.0, 0.0, 1.0);
	}
		
	if(maxComponent < abs(fragFromLight.z))
	{
		maxComponent = abs(fragFromLight.z);
		samplingVec1 = vec3(1.0, 0.0, 0.0);
		samplingVec2 = vec3(0.0, 1.0, 0.0);
	}
	
	vec3 samplingFragFromLight;
	for(int s1 = -sampleRadius; s1 <= sampleRadius; s1++)
	{
		samplingFragFromLight = fragFromLight + s1 * samplingVec1 * pixelSize;
		for(int s2 = -sampleRadius; s2 <= sampleRadius; s2++)
		{
			samplingFragFromLight += s2 * samplingVec2 * pixelSize;
			closestDepth = texture(light.shadowMap, samplingFragFromLight).r;
			closestDepth *= pointLightFarPlane;
			if(currentDepth > closestDepth + bias)
				shadow += 1.0;
		}
	}
	shadow /= (2 * sampleRadius + 1) * (2 * sampleRadius + 1);
	*/
	return shadow;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightVec = fragPos - light.position;
	float dist = length(lightVec);
	vec3 lightDir = normalize(lightVec);

	float diff = max(0.0, dot(normal, -lightDir));
	vec3 reflectDir = reflect(lightDir, normal);
	float spec = pow(max(0.0, dot(viewDir, reflectDir)), 32);

	float attenuation = 1.0 / (light.constant + (light.linear + light.quadratic * dist) * dist);
	
	vec3 ambient = light.ambient * vec3(texture(diffuse0, texCoord)) * attenuation;
	vec3 diffuse = light.diffuse * diff * vec3(texture(diffuse0, texCoord)) * attenuation;
	vec3 specular = light.specular * spec * vec3(texture(specular0, texCoord).r) * attenuation;

	return ambient + (diffuse + specular) * (1.0 - CalcPointShadow(light, fragPos));
}



vec4 directLight()
{
	vec3 lightDirection =  normalize(vec3(0.0, -1.0, 0.0));
	//vec4 lightColor = vec4(0.6, 0.6, 0.6, 1.0);
	vec4 lightColor = vec4(1.0, 1.0, 1.0, 1.0);

	vec3 normal = normalize(Normal);

	float diffuse = max(0.0f, dot(normal, -lightDirection));

	float ambient = 0.20f;

	float specularLight = 0.50f;
	vec3 viewDirection  = normalize(camPos - crntPos);
	vec3 reflection = normalize(reflect(lightDirection, normal));
	float specAmount = pow(max(0.0f, dot(viewDirection, reflection)), 32);
	float specular = specAmount * specularLight;

	float shadow = 0.0;
	vec3 lightCoords = posDirLightVP.xyz / posDirLightVP.w;
	float bias = max(0.025 * (1.0 - dot(normal, lightDirection)), 0.0005);
	if(lightCoords.z <= 1.0 && lightCoords.z >= -1.0)
	{
		// from [-1, 1] to [0, 1]
		lightCoords = (lightCoords + 1.0) / 2.0;
		float currentDepth = lightCoords.z;
		

		int sampleRadius = 2;
		vec2 pixelSize = 1.0 / textureSize(shadowMap, 0);
		for(int y = -sampleRadius; y<=sampleRadius; y++)
			for(int x = -sampleRadius; x<=sampleRadius; x++)
			{
				float closestDepth = texture(shadowMap, lightCoords.xy + vec2(x, y) * pixelSize).r;
				if(currentDepth > closestDepth + bias)
					shadow += 1.0;
			}
			shadow /= ((2.0 * float(sampleRadius) + 1.0) * (2.0 * float(sampleRadius) + 1.0));

	}
	return texture(diffuse0, texCoord) * lightColor * ((diffuse + specular) * (1.0 - shadow) + ambient );
}

vec4 spotLight()
{
	
	vec3 lightVec = crntPos - lightPos;
	float dist = length(lightVec);
	float a = 0.03f;
	float b = 0.09f;
	float inten = 1.0f / (a * dist * dist + b * dist + 1.0f);

	vec3 normal = normalize(Normal);
	vec3 lightDirection =  normalize(lightVec);

	float diffuse = max(0.0f, dot(normal, -lightDirection));

	float ambient = 0.20f;

	float specularLight = 0.90f;
	vec3 viewDirection  = normalize(camPos - crntPos);
	vec3 reflection = normalize(reflect(lightDirection, normal));
	float specAmount = pow(max(0.0f, dot(viewDirection, reflection)), 32);
	float specular = specAmount * specularLight;

	float outerCone = 0.90;
	float innerCone = 0.95;

	float angle = dot(vec3(0.0f, -1.0f, 0.0f), lightDirection);
	float inten2 = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);

	return texture(diffuse0, texCoord) * lightColor * ((diffuse + specular) * inten * inten2 + ambient );
}

vec3 FlashLight(vec3 normal, vec3 fragPos, vec3 viewDir, vec3 camPos, vec3 camDir)
{
	vec3 lightPos = camPos;
	vec3 lightVec = fragPos - lightPos;
	float dist = length(lightVec);
	vec3 lightDir = normalize(lightVec);

	float a = 0.04f;
	float b = 0.03f;
	float attenuation= 1.0f / (a * dist * dist + b * dist + 1.0f);

	float diff = max(0.0, dot(normal, -lightDir));
	vec3 reflectDir = normalize(reflect(lightDir, normal));
	float spec = pow(max(0.0, dot(viewDir, reflectDir)), 64);

	vec3 color = vec3(0.9, 0.9, 0.9);

	float outerCone = 0.96;
	float innerCone = 0.975;

	float angle = dot(camDir, lightDir);
	float inten = 2.0 * clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);
	inten *= 0.7;
	
	// vec3 ambient = light.ambient * vec3(texture(diffuse0, texCoord)) * attenuation * inten;
	vec3 diffuse = color * diff * vec3(texture(diffuse0, texCoord)) * attenuation * inten;
	vec3 specular = color * spec * vec3(texture(specular0, texCoord).r) * attenuation * inten;
	return diffuse + specular;
}

void main()
{
	vec3 normal = normalize(Normal);
	vec3 viewDir  = normalize(camPos - crntPos);
	vec3 col = vec3(0.0, 0.0, 0.0);
	for(int i = 0; i < MAX_POINT_LIGHT_NR; i++)
	{
		col += CalcPointLight(pointLights[i], normal, crntPos, viewDir);
	}
	if(flashLight)
	{
		col += FlashLight(normal, crntPos, viewDir, camPos, camDir);
	}
	// col += vec3(directLight());
	FragColor = vec4(col, 1.0);
	// vec3 fragFromLight = crntPos - pointLights[1].position;
	// float closestDepth = texture(pointLights[1].shadowMap, fragFromLight).r;
	// FragColor = vec4(vec3(closestDepth), 1.0);
} 