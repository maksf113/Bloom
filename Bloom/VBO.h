#pragma once
#include <gl/glew.h>
#include <glm/glm.hpp>
#include <vector>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texUV;
};

struct ClothVertex
{
	glm::vec3 position;
	glm::vec3 previousPosition;
	glm::vec3 force;
	glm::vec2 texUV;
};

template<typename vertexType>
class VBO 
{
private:
	GLuint m_id;
public:
	VBO(){}
	VBO(std::vector<vertexType>& vertices)
	{
		glGenBuffers(1, &m_id);
		glBindBuffer(GL_ARRAY_BUFFER, m_id); // ????
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertexType), vertices.data(), GL_STATIC_DRAW);
	}
	VBO(GLfloat* vertices, GLsizeiptr size)
	{
		glGenBuffers(1, &m_id);
		glBindBuffer(GL_ARRAY_BUFFER, m_id); // ????
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}
	VBO(std::vector<vertexType>& vertices, GLenum mode)
	{
		glGenBuffers(1, &m_id);
		glBindBuffer(GL_ARRAY_BUFFER, m_id); // ????
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertexType), vertices.data(), mode);
	}
	GLuint id() const { return m_id; }
	void Bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_id);
	}
	void Unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	void Delete()
	{
		glDeleteBuffers(1, &m_id);
	}
};


