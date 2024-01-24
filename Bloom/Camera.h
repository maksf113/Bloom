#pragma once
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include "ShaderProgram.h"

class Camera
{
private:
	glm::vec3 m_position;
	glm::vec3 m_direction = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 m_up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 m_view = glm::mat4(1.0f);
	glm::mat4 m_projection = glm::mat4(1.0f);
	glm::mat4 m_cameraMatrix = glm::mat4(1.0f);
	float m_FOVdeg = 45.0f;
	float m_nearPlane = 0.1f;
	float m_farPlane = 100.0f;
	GLuint m_width;
	GLuint m_height;

	float m_speed = 0.005f;
	float m_sensitivity = 30.0f;

	bool m_firstClickEsc = true;
	bool m_locked = true;
	bool m_firstClickF = true;
	bool m_flashLight = false;
	bool m_firstClickLeft = true;

public:
	Camera() {}
	Camera(int width, int height, glm::vec3 position);
	// update camera + send to vert shader
	void updateMatrix(float FOVdeg, float nearPlane, float farPlane);
	void Matrix(ShaderProgram& shader, const char* uniform);
	void SetUniforms(const ShaderProgram& shader, const char* uniMatrix, const char* uniPos) const;
	void Input(GLFWwindow* window, float dt);
	//void Input(GLFWwindow* window, PickFBO& pickFBO, glm::vec2& translationInPixels);
	glm::vec2 CursorClickPosition(GLFWwindow* window);
	glm::vec2 CursorPosition(GLFWwindow* window);
	glm::vec3 GetDirection() const { return m_direction; }
	glm::vec3 GetPosition() const { return m_position; }
	glm::vec3 GetUp() const { return m_up; }
	glm::mat4 GetVP() const { return m_cameraMatrix; }
	glm::mat4 GetProjection() const { return m_projection; }
	glm::mat4 GetView() const { return m_view; }
	float GetFOVdeg() const { return m_FOVdeg; }
	float GetNearPlane() const { return m_nearPlane; }
	float GetFarPlane() const { return m_farPlane; }
	GLuint GetWidth() const { return m_width; }
	GLuint GetHeight() const { return m_height; }
	bool GetFlashlight() const { return m_flashLight; }
	void SetFOVdeg(float fovDeg) { m_FOVdeg = fovDeg; }
	void SetPos(glm::vec3 pos) { m_position = pos; }
	void SetDir(glm::vec3 dir) { m_direction = dir; }
	void SetUp(glm::vec3 up) { m_up = up; }
	void SetFarPlane(float FarPlane) { m_farPlane = FarPlane; }
	bool IsLocked() const { return m_locked; }
	bool IsFirstClickLeft() const { return m_firstClickLeft; }
};