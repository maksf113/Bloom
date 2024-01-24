#include "PointLight.h"

Mesh<Vertex> PointLight::SetMesh()
{
	Vertex lightVertices[8] =
	{ //     COORDINATES     //
		Vertex{glm::vec3(-0.1f, -0.1f,  0.1f)},
		Vertex{glm::vec3(-0.1f, -0.1f, -0.1f)},
		Vertex{glm::vec3(0.1f, -0.1f, -0.1f)},
		Vertex{glm::vec3(0.1f, -0.1f,  0.1f)},
		Vertex{glm::vec3(-0.1f,  0.1f,  0.1f)},
		Vertex{glm::vec3(-0.1f,  0.1f, -0.1f)},
		Vertex{glm::vec3(0.1f,  0.1f, -0.1f)},
		Vertex{glm::vec3(0.1f,  0.1f,  0.1f)}
	};

	GLuint lightIndices[36] =
	{
		0, 1, 2,
		0, 2, 3,
		0, 4, 7,
		0, 7, 3,
		3, 7, 6,
		3, 6, 2,
		2, 6, 5,
		2, 5, 1,
		1, 5, 4,
		1, 4, 0,
		4, 5, 6,
		4, 6, 7
	};

	std::vector <Vertex> lightVerts(lightVertices, lightVertices + sizeof(lightVertices) / sizeof(Vertex));
	std::vector <GLuint> lightInd(lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));
	Mesh<Vertex> light(lightVerts, lightInd);
	return light;
}

void PointLight::SetLightUniforms(ShaderProgram& lightShader)
{
	lightShader.Activate();
	glm::mat4 posMat = glm::translate(glm::mat4(1.0f), position);
	lightShader.setMat4("model", posMat);
	lightShader.setVec4("lightColor", glm::vec4(color, 1.0f));
}

void PointLight::SetModelUniforms(ShaderProgram& shader)
{
	shader.Activate();
	shader.setVec3("pointLights[" + std::to_string(i) + "].position", position);

	shader.setFloat("pointLights[" + std::to_string(i) + "].constant", constant);
	shader.setFloat("pointLights[" + std::to_string(i) + "].linear", linear);
	shader.setFloat("pointLights[" + std::to_string(i) + "].quadratic", quadratic);

	shader.setVec3("pointLights[" + std::to_string(i) + "].ambient", ambient);
	shader.setVec3("pointLights[" + std::to_string(i) + "].diffuse", diffuse);
	shader.setVec3("pointLights[" + std::to_string(i) + "].specular", specular);

	//shader.setInt("pointLights[" + std::to_string(i) + "].shadowMap", shadowMap);
}
