#pragma once
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>

std::string readShaderSource(const char*);
void printShaderLog(GLuint shader);
void printProgramLog(int prog);
bool checkOpenGLError();
static void GLclearError() { while (glGetError() != GL_NO_ERROR); }
static void GLcheckError() {
	while (GLenum error = glGetError())
		std::cout << "[OpenGL Error] " << error << std::endl;
}

class ShaderProgram
{
private:
	// Reference ID of the Shader Program
	GLuint m_id;
public:
	// Constructor that build the Shader Program from 2 different shaders
	ShaderProgram() {}
	ShaderProgram(const char* vertexFile, const char* fragmentFile);
	ShaderProgram(const char* vertexFile);
	ShaderProgram(const char* vertexFile, const char* fragmentFile, const char* geometryFile);
	const GLuint id() const;
	// Activates the Shader Program
	void Activate() const;
	// Deletes the Shader Program
	void Delete();
	// Set uniforms
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setUint(const std::string& name, GLuint value) const;
	void setFloat(const std::string& name, float value) const;
	void setVec2(const std::string& name, glm::vec2 value) const;
	void setVec3(const std::string& name, glm::vec3 value) const;
	void setVec4(const std::string& name, glm::vec4 value) const;
	void setMat3(const std::string& name, glm::mat3 value) const;
	void setMat4(const std::string& name, glm::mat4 value) const;

};

