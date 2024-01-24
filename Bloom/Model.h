#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mesh.h"

class Model 
{
private:
	std::vector<Mesh<Vertex>> m_meshes;
	std::string m_directory;
	std::vector<Texture> m_textures_loaded;

	void loadModel(const std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh<Vertex> processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
public:
	Model(const char* path) 
	{
		loadModel(path);
	}
	void Draw(const ShaderProgram& shader, const Camera& camera)
	{
		for (GLuint i = 0; i < m_meshes.size(); i++)
		{
			m_meshes[i].Draw(shader, camera);
		}
	}
};

