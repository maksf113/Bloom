#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTex;

out vec3 crntPos;
out vec3 Normal;
out vec3 color;
out vec2 texCoord;
out vec4 posDirLightVP;

uniform mat4 camMatrix;
uniform mat4 model;
uniform mat4 dirLightVP;


void main()
{
	crntPos = vec3(model * vec4(aPos, 1.0f));
    Normal = aNormal;
    texCoord = aTex;

    posDirLightVP = dirLightVP * model * vec4(aPos, 1.0);

    gl_Position = camMatrix * vec4(crntPos, 1.0); 
}