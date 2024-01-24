#pragma once
#include "Mesh.h"

class PointLight
{
public:
	int i;
	glm::vec3 position;
	glm::vec3 color;

	float constant;
	float linear;
	float quadratic;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	GLuint shadowMap;

	Mesh<Vertex> mesh = SetMesh();
	PointLight(int it, glm::vec3 pos, glm::vec3 col) : i(it), position(pos), color(col)
	{
		ambient = col / 15.0f; 
		diffuse = col;
		specular = col * 0.1f;
		constant = 1.0;
		linear = 0.2f;
		quadratic = 0.1f;
	}
	PointLight() : position(glm::vec3(0.0f, 0.0f, 0.0f)), ambient(glm::vec3(0.0f, 0.0f, 0.0f)),
	diffuse(glm::vec3(0.0f, 0.0f, 0.0f)) , specular(glm::vec3(0.0f, 0.0f, 0.0f))
	{
		constant = 1.0;
		linear = 0.09f;
		quadratic = 0.032f;
	}
	void SetPos(glm::vec3 Pos) { position = Pos; }
	void SetCol(glm::vec3 Col) { ambient = Col; diffuse = Col; specular = Col; }
	void SetI(int it) { i = it; }
	void SetShadowMap(GLuint shadowTexture) { shadowMap = shadowTexture; }
	void BindShadowTexture() { glBindTexture(GL_TEXTURE_CUBE_MAP, shadowMap); }
	void SetLightUniforms(ShaderProgram& lightShader);
	void SetModelUniforms(ShaderProgram& modelShader);
	Mesh<Vertex> SetMesh();
	void Draw(ShaderProgram& shader, Camera& camera) { mesh.Draw(shader, camera); }
};





