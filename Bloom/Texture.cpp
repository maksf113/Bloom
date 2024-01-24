#include "Texture.h"

Texture::Texture(const char* image, const char* texType, GLuint slot) 
{
	m_type = texType;
	int widthImg, heightImg, numColCh;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0); 
	if (bytes == NULL) 
		std::cout << "Failed to load image" << std::endl;
	glGenTextures(1, &m_id);;
	glActiveTexture(GL_TEXTURE0 + slot);
	m_unit = slot;
	glBindTexture(GL_TEXTURE_2D, m_id);
	// scaling
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// fitting
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	

	// assign the image to the current OpenGL texture object
	GLenum format;
	if (numColCh == 1)
		format = GL_RED;
	else if (numColCh == 3)
		format = GL_RGB;
	else if (numColCh == 4)
		format = GL_RGBA;

	//stbi_set_flip_vertically_on_load(true);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg,
		0, format, GL_UNSIGNED_BYTE, bytes);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(bytes);
	glBindTexture(GL_TEXTURE_2D, 0);
	
}

Texture::Texture(aiString sPath, std::string directory, std::string texType, GLuint slot)
{
	m_type = texType.c_str();
	path = sPath;
	std::string fileName = std::string(path.C_Str());
	fileName = directory + '/' + fileName;
	int widthImg, heightImg, numColCh;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* bytes = stbi_load(fileName.c_str(), &widthImg, &heightImg, &numColCh, 0);
	if (bytes == NULL)
		std::cout << "Failed to load image: " << fileName << std::endl;
	glGenTextures(1, &m_id);;
	glActiveTexture(GL_TEXTURE0 + slot);
	m_unit = slot;
	glBindTexture(GL_TEXTURE_2D, m_id);
	// scaling
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// fitting
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


	// assign the image to the current OpenGL texture obkject
	GLenum format;
	if (numColCh == 1)
		format = GL_RED;
	else if (numColCh == 3)
		format = GL_RGB;
	else if (numColCh == 4)
		format = GL_RGBA;

	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg,
		0, format, GL_UNSIGNED_BYTE, bytes);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(bytes);
	glBindTexture(GL_TEXTURE_2D, 0);

}

void Texture::texUnit(const ShaderProgram& shader, const char* uniform, GLuint unit)
{
	// get location of the uniform
	GLuint texUni = glGetUniformLocation(shader.id(), uniform);
	// shader needs to be activated befor changing the value of a uniform !!!!!!!!!!!!!
	shader.Activate();
	glUniform1i(texUni, unit);
}
void Texture::Bind()
{
	glActiveTexture(GL_TEXTURE0 + m_unit);
	glBindTexture(GL_TEXTURE_2D, m_id);
}
void Texture::Unbind() 
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
void Texture::Delete() 
{
	glDeleteTextures(1, &m_id);
}