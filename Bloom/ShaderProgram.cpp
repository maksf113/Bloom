#include "ShaderProgram.h"

void printShaderLog(GLuint shader)
{
	int len = 0;
	int chWrittn = 0;
	char* log;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
	if (len > 0) 
	{
		log = (char*)malloc(len);
		glGetShaderInfoLog(shader, len, &chWrittn, log);
		std::cout << "Shader Info Log: " << log << std::endl;
		free(log);
	}
}
void printProgramLog(int prog) 
{
	int len = 0;
	int chWrittn = 0;
	char* log;
	glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
	if (len > 0) 
	{
		log = (char*)malloc(len);
		glGetProgramInfoLog(prog, len, &chWrittn, log);
		std::cout << "Program Info Log: " << log << std::endl;
		free(log);
	}
}
bool checkOpenGLError() 
{
	bool foundError = false;
	int glErr = glGetError();
	while (glErr != GL_NO_ERROR)
	{
		std::cout << "glError: " << glErr << std::endl;
		foundError = true;
		glErr = glGetError();
	}
	return foundError;
}

// Constructor that build the Shader Program from 2 different shaders
ShaderProgram::ShaderProgram(const char* vertexFile, const char* fragmentFile)
{
	// Read vertexFile and fragmentFile and store the strings
	std::string vertexCode = readShaderSource(vertexFile);
	std::string fragmentCode = readShaderSource(fragmentFile);

	// Convert the shader source strings into character arrays
	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

	GLint vertexCompiled;
	GLint fragmentCompiled;
	GLint linked;

	// Create Vertex Shader Object and get its reference
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Attach Vertex Shader source to the Vertex Shader Object
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	// Compile the Vertex Shader into machine code
	glCompileShader(vertexShader);
	// Errors
	//checkOpenGLError();
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexCompiled);
	if (vertexCompiled != 1) {
		std::cout << "vertex compilation failed" << std::endl;
		printShaderLog(vertexShader);
	}

	// Create Fragment Shader Object and get its reference
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Attach Fragment Shader source to the Fragment Shader Object
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	// Compile the Vertex Shader into machine code
	glCompileShader(fragmentShader);
	// Errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentCompiled);
	if (fragmentCompiled != 1) {
		std::cout << "fragment compilation failed" << std::endl;
		printShaderLog(fragmentShader);
	}
	// Create Shader Program Object and get its reference
	m_id = glCreateProgram();
	// Attach the Vertex and Fragment Shaders to the Shader Program
	glAttachShader(m_id, vertexShader);
	glAttachShader(m_id, fragmentShader);
	// Wrap-up/Link all the shaders together into the Shader Program
	glLinkProgram(m_id);
	// Errors
	//checkOpenGLError();
	glGetProgramiv(m_id, GL_LINK_STATUS, &linked);
	if (linked != 1) {
		std::cout << "linking failed" << std::endl;
		printProgramLog(m_id);
	}

	// Delete the now useless Vertex and Fragment Shader objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

}

ShaderProgram::ShaderProgram(const char* vertexFile)
{
	// Read vertexFile and fragmentFile and store the strings
	std::string vertexCode = readShaderSource(vertexFile);

	// Convert the shader source strings into character arrays
	const char* vertexSource = vertexCode.c_str();

	GLint vertexCompiled;
	GLint linked;

	// Create Vertex Shader Object and get its reference
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Attach Vertex Shader source to the Vertex Shader Object
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	// Compile the Vertex Shader into machine code
	glCompileShader(vertexShader);
	// Errors
	//checkOpenGLError();
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexCompiled);
	if (vertexCompiled != 1) {
		std::cout << "vertex compilation failed" << std::endl;
		printShaderLog(vertexShader);
	}
	// Create Shader Program Object and get its reference
	m_id = glCreateProgram();
	// Attach the Vertex and Fragment Shaders to the Shader Program
	glAttachShader(m_id, vertexShader);
	// Wrap-up/Link all the shaders together into the Shader Program
	glLinkProgram(m_id);
	// Errors
	//checkOpenGLError();
	glGetProgramiv(m_id, GL_LINK_STATUS, &linked);
	if (linked != 1) {
		std::cout << "linking failed" << std::endl;
		printProgramLog(m_id);
	}

	// Delete the now useless Vertex and Fragment Shader objects
	glDeleteShader(vertexShader);

}

ShaderProgram::ShaderProgram(const char* vertexFile, const char* fragmentFile, const char* geometryFile)
{
	// Read vertexFile and fragmentFile and store the strings
	std::string vertexCode = readShaderSource(vertexFile);
	std::string fragmentCode = readShaderSource(fragmentFile);
	std::string geometryCode = readShaderSource(geometryFile);

	// Convert the shader source strings into character arrays
	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();
	const char* geometrySource = geometryCode.c_str();

	GLint vertexCompiled;
	GLint fragmentCompiled;
	GLint geometryCompiled;
	GLint linked;

	// Create Vertex Shader Object and get its reference
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Attach Vertex Shader source to the Vertex Shader Object
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	// Compile the Vertex Shader into machine code
	glCompileShader(vertexShader);
	// Errors
	//checkOpenGLError();
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexCompiled);
	if (vertexCompiled != 1) 
	{
		std::cout << "vertex compilation failed" << std::endl;
		printShaderLog(vertexShader);
	}

	// Create Fragment Shader Object and get its reference
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Attach Fragment Shader source to the Fragment Shader Object
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	// Compile the Vertex Shader into machine code
	glCompileShader(fragmentShader);
	// Errors
	glCompileShader(fragmentShader);
	//checkOpenGLError();
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentCompiled);
	if (fragmentCompiled != 1) {
		std::cout << "fragment compilation failed" << std::endl;
		printShaderLog(fragmentShader);
	}

	// Create Fragment Shader Object and get its reference
	GLuint geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
	// Attach Fragment Shader source to the Fragment Shader Object
	glShaderSource(geometryShader, 1, &geometrySource, NULL);
	// Compile the Vertex Shader into machine code
	glCompileShader(geometryShader);
	// Errors
	glCompileShader(geometryShader);
	//checkOpenGLError();
	glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &geometryCompiled);
	if (geometryCompiled != 1) {
		std::cout << "geometry compilation failed" << std::endl;
		printShaderLog(geometryShader);
	}

	// Create Shader Program Object and get its reference
	m_id = glCreateProgram();
	// Attach the Vertex and Fragment Shaders to the Shader Program
	glAttachShader(m_id, vertexShader);
	glAttachShader(m_id, fragmentShader);
	glAttachShader(m_id, geometryShader);
	// Wrap-up/Link all the shaders together into the Shader Program
	glLinkProgram(m_id);
	// Errors
	//checkOpenGLError();
	glGetProgramiv(m_id, GL_LINK_STATUS, &linked);
	if (linked != 1) {
		std::cout << "linking failed" << std::endl;
		printProgramLog(m_id);
	}

	// Delete the now useless Vertex and Fragment Shader objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(geometryShader);
}

// Activates the Shader Program
void ShaderProgram::Activate() const
{
	glUseProgram(m_id);
}

// Deletes the Shader Program
void ShaderProgram::Delete()
{
	glDeleteProgram(m_id);
}

const GLuint ShaderProgram::id() const
{
	return m_id;
}

std::string readShaderSource(const char* filePath) {
	std::string content;
	std::ifstream fileStream(filePath, std::ios::in);
	std::string line = "";
	while (!fileStream.eof()) {
		getline(fileStream, line);
		content.append(line + "\n");
	}
	fileStream.close();
	return content;
}

void ShaderProgram::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(m_id, name.c_str()), int(value));
}
void ShaderProgram::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
}
void ShaderProgram::setUint(const std::string& name, GLuint value) const
{
	glUniform1ui(glGetUniformLocation(m_id, name.c_str()), value);
}
void ShaderProgram::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(m_id, name.c_str()), value);
}
void ShaderProgram::setVec2(const std::string& name, glm::vec2 value) const 
{
	glUniform2f(glGetUniformLocation(m_id, name.c_str()), value.x, value.y);
}
void ShaderProgram::setVec3(const std::string& name, glm::vec3 value) const 
{
	glUniform3f(glGetUniformLocation(m_id, name.c_str()), value.x, value.y, value.z);
}
void ShaderProgram::setVec4(const std::string& name, glm::vec4 value) const 
{
	glUniform4f(glGetUniformLocation(m_id, name.c_str()), value.r, value.g, value.b, value.a);
}
void ShaderProgram::setMat3(const std::string& name, glm::mat3 value) const
{
	glUniformMatrix3fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}
void ShaderProgram::setMat4(const std::string& name, glm::mat4 value) const 
{
	glUniformMatrix4fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

