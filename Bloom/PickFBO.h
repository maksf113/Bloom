#pragma once
#include "FBO.h"
#include "Camera.h"

class PickFBO : public FBO
{
public:
	PickFBO() {}
	PickFBO(GLuint width, GLuint height);
	~PickFBO();

	void Init(GLuint width, GLuint height);

	struct PixelInfo
	{
		GLuint objectID;
		GLuint drawID;
		GLuint primitiveID;

		void Print()
		{
			std::cout << "Object: " << objectID << std::endl;
			std::cout << "Draw: " << drawID << std::endl;
			std::cout << "Primitive: " << primitiveID << std::endl;
		}
	};

	PixelInfo ReadPixel(GLuint x, GLuint y);
	bool Clicked(GLFWwindow* window, Camera& camera);
	glm::vec3 UpdateObjectPosition(GLFWwindow* window, glm::vec3 crntObjPosition, glm::vec2 cursor, Camera camera);
};
 