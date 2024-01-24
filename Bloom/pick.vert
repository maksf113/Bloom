#version 430 core

layout(location = 0) in vec3 aPosition;

uniform mat4 camMatrix;
uniform mat4 model;

void main()
{
	gl_Position = camMatrix * model * vec4(aPosition, 1.0);
}