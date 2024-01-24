#version 430 core

layout(location = 0) out uvec3 Indices;

uniform uint objectIndex;
uniform uint drawIndex;

void main()
{
	Indices = uvec3(objectIndex, drawIndex, gl_PrimitiveID);
}