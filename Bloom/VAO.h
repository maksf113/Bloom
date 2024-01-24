#pragma once
#include <gl/glew.h>
#include "VBO.h"

template<typename vertexType>
class VAO 
{
private:
	GLuint m_id;
public:
	VAO()
	{
		glGenVertexArrays(1, &m_id);
	}
	GLuint id()
	{
		return m_id;
	}
	void linkAttrib(VBO<vertexType>& VBO, GLuint layout, GLuint numComponents, GLenum type, 
		GLsizeiptr stride, void* offset)
	{
		VBO.Bind();
		glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
		glEnableVertexAttribArray(layout);
		VBO.Unbind();
	}
	void Bind()
	{
		glBindVertexArray(m_id);
	}
	void Unbind()
	{
		glBindVertexArray(0);
	}
	void Delete()
	{
		glDeleteVertexArrays(1, &m_id);
	}
};
