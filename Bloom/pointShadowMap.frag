#version 430 core

in vec4 FragPos;

uniform vec3 lightPos;
uniform float pointLightFarPlane;

void main() 
{
	float lightDistance = length(FragPos.xyz - lightPos);

	lightDistance /= pointLightFarPlane; // map range to [0, 1]

	gl_FragDepth = lightDistance;
}