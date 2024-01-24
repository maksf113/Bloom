/*
#include "VAO.h"

VAO::VAO()
{
	glGenVertexArrays(1, &m_id);
}
GLuint VAO::id()
{
	return m_id;
}
void VAO::linkAttrib(VBO<Vertex>& VBO, GLuint layout, GLuint numComponents,
	GLenum type, GLsizeiptr stride, void* offset)
{
	VBO.Bind();
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);
	VBO.Unbind();
}
void VAO::Bind()
{
	glBindVertexArray(m_id);
}
void VAO::Unbind()
{
	glBindVertexArray(0);
}
void VAO::Delete()
{
	glDeleteVertexArrays(1, &m_id);
}
*/