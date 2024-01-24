#pragma once
#include <string>

#include "VAO.h"
#include "EBO.h"
#include "Camera.h"
#include "Texture.h"

template<typename vertexType>
class Mesh 
{
protected:
	std::vector<vertexType> m_vertices;
	std::vector<GLuint> m_indices;
	std::vector<Texture> m_textures;
	VAO<vertexType> m_vao;

	void CreateGLState(std::vector<vertexType>& vertices, std::vector<GLuint>& indices);

public:
	Mesh() {}
	Mesh(std::vector<vertexType>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures);
	Mesh(std::vector<vertexType>& vertices, std::vector<GLuint>& indices);
	void virtual Draw(const ShaderProgram& shader, const Camera& camera);
};

template<typename vertexType>
Mesh<vertexType>::Mesh(std::vector<vertexType>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures)
{
	m_vertices = vertices;
	m_indices = indices;
	m_textures = textures;
	CreateGLState(vertices, indices);
}

template<typename vertexType>
Mesh<vertexType>::Mesh(std::vector<vertexType>& vertices, std::vector<GLuint>& indices)
{
	m_vertices = vertices;
	m_indices = indices;
	CreateGLState(vertices, indices);
}

template<typename vertexType>
void Mesh<vertexType>::CreateGLState(std::vector<vertexType>& vertices, std::vector<GLuint>& indices)
{
	m_vao.Bind();
	VBO<vertexType> vbo(vertices);
	EBO ebo(indices);
	// link position, normal, texCoord
	m_vao.linkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(vertexType), (void*)0);
	m_vao.linkAttrib(vbo, 1, 3, GL_FLOAT, sizeof(vertexType), (void*)(3 * sizeof(float)));
	m_vao.linkAttrib(vbo, 2, 2, GL_FLOAT, sizeof(vertexType), (void*)(6 * sizeof(float)));

	m_vao.Unbind();
	vbo.Unbind();
	ebo.Unbind();
}
// declaration for method specialization in cpp file
template<>
void Mesh<ClothVertex>::CreateGLState(std::vector<ClothVertex>& vertices, std::vector<GLuint>& indices);

template<typename vertexType>
void Mesh<vertexType>::Draw(const ShaderProgram& shader, const Camera& camera)
{
	shader.Activate();
	m_vao.Bind();

	// number of textures (diffuse) and specular maps
	unsigned int numDiffuse = 0;
	unsigned int numSpecular = 0;
	unsigned int numNormal = 0;

	for (unsigned int i = 0; i < m_textures.size(); i++)
	{
		std::string num;
		std::string type = m_textures[i].Type();

		if (type == "diffuse")
			num = std::to_string(numDiffuse++);

		else if (type == "specular")
			num = std::to_string(numSpecular++);

		else if (type == "normal")
			num = std::to_string(numNormal++);

		m_textures[i].texUnit(shader, (type + num).c_str(), i);
		m_textures[i].Bind();
	}

	camera.SetUniforms(shader, "camMatrix", "camPos");

	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
}