#pragma once
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <stb\stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "ShaderProgram.h"

class Texture {
private:
	GLuint m_id;
	const char* m_type;
	GLuint m_unit;
	
public:
	aiString path;
	Texture(const char* image, const char* texType, GLuint slot);
	Texture(aiString sPath, std::string directory, std::string texType, GLuint slot);

	// assign texture unit to a texture
	void texUnit(const ShaderProgram& shader, const char* uniform, GLuint unit);
	GLuint id() const { return m_id; }
	const char* Type() const { return m_type; }
	void Bind();
	void Unbind();
	void Delete();

};