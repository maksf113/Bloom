#include "Mesh.h"

template<>
void Mesh<ClothVertex>::CreateGLState(std::vector<ClothVertex>& vertices, std::vector<GLuint>& indices)
{
	m_vao.Bind();
	VBO<ClothVertex> vbo(vertices);
	EBO ebo(indices);
	// link position, previous position, force and texUVs
	m_vao.linkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(ClothVertex), (void*)0);
	m_vao.linkAttrib(vbo, 1, 3, GL_FLOAT, sizeof(ClothVertex), (void*)(3 * sizeof(float)));
	m_vao.linkAttrib(vbo, 2, 3, GL_FLOAT, sizeof(ClothVertex), (void*)(6 * sizeof(float)));
	m_vao.linkAttrib(vbo, 3, 2, GL_FLOAT, sizeof(ClothVertex), (void*)(9 * sizeof(float)));

	m_vao.Unbind();
	vbo.Unbind();
	ebo.Unbind();
}