#pragma once
#include "Mesh.h"

class Floor
{
private:
	float m_size;
	Mesh<Vertex> m_mesh;
	// Vertices coordinates
	Vertex vertices[4] =
	{ //               COORDINATES           /           NORMALS         /       TEXTURE COORDINATES    //
		Vertex{glm::vec3(-m_size, 0.0f,  m_size), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
		Vertex{glm::vec3(-m_size, 0.0f, -m_size), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 10.0f)},
		Vertex{glm::vec3(m_size, 0.0f, -m_size), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(10.0f, 10.0f)},
		Vertex{glm::vec3(m_size, 0.0f,  m_size), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(10.0f, 0.0f)}
	};

	// Indices for vertices order
	GLuint indices[6] =
	{
		0, 1, 2,
		0, 2, 3
	};
public:
	Floor(float Size, const char* textureDiffuse, const char* textureSpecular) : m_size(Size)
	{
		Texture textures[2]
		{
			Texture(textureDiffuse, "diffuse0", 0),
			Texture(textureSpecular, "specular0", 1)
		};
		std::vector <Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
		std::vector <GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));
		std::vector <Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));
		m_mesh = Mesh<Vertex>(verts, ind, tex);
	}
	void Draw(const ShaderProgram& shader, const Camera& camera)
	{
		m_mesh.Draw(shader, camera);
	}
};
