#pragma once
#include <gl/glew.h>
#include <vector>

class EBO 
{
private:
	GLuint m_id;
public:
	EBO() {};
	EBO(std::vector<GLuint>& indices);
	EBO(GLuint* indices, GLsizeiptr size);
	GLuint id() const;
	void Bind();
	void Unbind();
	void Delete();
}; 