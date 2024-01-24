#version 430 core
out vec4 FragColor;

in vec3 crntPos;
in vec3 Normal;
in vec3 color;
in vec2 texCoord;

uniform sampler2D diffuse0;
uniform sampler2D specular0;

uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;


vec4 pointLight(){
	
	vec3 lightVec = crntPos - lightPos;
	float dist = length(lightVec);
	float a = 0.3f;
	float b = 0.1f;
	float inten = 1.0f / (a * dist * dist + b * dist + 1.0f);

	vec3 normal = normalize(Normal);
	vec3 lightDirection =  normalize(lightVec);

	float diffuse = max(0.0f, dot(normal, -lightDirection));

	float ambient = 0.20f;

	float specularLight = 0.50f;
	vec3 viewDirection  = normalize(camPos - crntPos);
	vec3 reflection = reflect(lightDirection, normal);
	float specAmount = pow(max(0.0f, dot(viewDirection, reflection)), 16);
	float specular = specAmount * specularLight;

	return texture(diffuse0, texCoord) * lightColor * (diffuse * inten + ambient) + texture(specular0, texCoord).r * specular * inten;
}

vec4 directLight(){
	vec3 lightDirection =  normalize(vec3(-1.0f, 0.0f, 0.0f));

	vec3 normal = normalize(Normal);

	float diffuse = max(0.0f, dot(normal, -lightDirection));

	float ambient = 0.20f;

	float specularLight = 0.90f;
	vec3 viewDirection  = normalize(camPos - crntPos);
	vec3 reflection = reflect(lightDirection, normal);
	float specAmount = pow(max(0.0f, dot(viewDirection, reflection)), 8);
	float specular = specAmount * specularLight;

	return texture(diffuse0, texCoord) * lightColor * (diffuse + specular + ambient );
}

vec4 spotLight(){
	
	vec3 lightVec = crntPos - lightPos;
	float dist = length(lightVec);
	float a = 3.0f;
	float b = 0.7f;
	float inten = 1.0f / (a * dist * dist + b * dist + 1.0f);

	vec3 normal = normalize(Normal);
	vec3 lightDirection =  normalize(lightVec);

	float diffuse = max(0.0f, dot(normal, -lightDirection));

	float ambient = 0.20f;

	float specularLight = 0.90f;
	vec3 viewDirection  = normalize(camPos - crntPos);
	vec3 reflection = reflect(lightDirection, normal);
	float specAmount = pow(max(0.0f, dot(viewDirection, reflection)), 8);
	float specular = specAmount * specularLight;

	float outerCone = 0.90;
	float innerCone = 0.95;

	float angle = dot(vec3(0.0f, -1.0f, 0.0f), lightDirection);
	float inten2 = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);

	return texture(diffuse0, texCoord) * lightColor * ((diffuse + specular) * inten * inten2 + ambient );
}

void main()
{
   FragColor = pointLight();
} 