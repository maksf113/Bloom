#include "Model.h"

void Model::loadModel(std::string path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		return;
	}
	this->m_directory = path.substr(0, path.find_last_of('/'));

	this->processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	for(GLuint i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* asmpMesh = scene->mMeshes[node->mMeshes[i]];
		Mesh<Vertex> mesh = processMesh(asmpMesh, scene);
		this->m_meshes.push_back(mesh);
	}

	for (GLuint i = 0; i < node->mNumChildren; i++)
		this->processNode(node->mChildren[i], scene);
}

Mesh<Vertex> Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

	for (GLuint i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 vector;

		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;

		vertex.position = vector;

		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;

		vertex.normal = vector;

		if (mesh->mTextureCoords[0]) // only use first texCoords (could be more)
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x; // 0-th coords of  ith vert
			vec.y = mesh->mTextureCoords[0][i].y;

			vertex.texUV = vec;
		}
		else
			vertex.texUV = glm::vec2(0.0f, 0.0f);

		vertices.push_back(vertex);
	}

	for (GLuint i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (GLuint j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<Texture> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE,
			"diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		std::vector<Texture> specularMaps = this->loadMaterialTextures(material, aiTextureType_SPECULAR,
			"specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	return Mesh<Vertex>(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;

	for (GLuint i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);

		GLboolean skip = false;
		for (GLuint j = 0; j < m_textures_loaded.size(); j++)
		{
			if (m_textures_loaded[j].path == str)
			{
				textures.push_back(m_textures_loaded[j]);
				skip = true;
				break;
			}
				
		}

		if (!skip)
		{
			if (typeName == "specular")
				i += mat->GetTextureCount(aiTextureType_DIFFUSE);
			Texture texture(str, m_directory, typeName, i); 

			textures.push_back(texture);
			this->m_textures_loaded.push_back(texture);
		}
	}
	return textures;
}