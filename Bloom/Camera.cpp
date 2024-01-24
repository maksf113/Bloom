#include "Camera.h"

Camera::Camera(int width, int height, glm::vec3 position)
{
	m_width = width;
	m_height = height;
	m_position = position;
}

void Camera::updateMatrix(float FOVdeg, float nearPlane, float farPlane)
{
	m_FOVdeg = FOVdeg;
	m_nearPlane = nearPlane;
	m_farPlane = farPlane;
	m_view = glm::lookAt(m_position, m_position + m_direction, m_up);
	m_projection = glm::perspective(glm::radians(m_FOVdeg), float(m_width) / float(m_height), m_nearPlane, m_farPlane);

	m_cameraMatrix = m_projection * m_view;
}
void Camera::SetUniforms(const ShaderProgram& shader, const char* uniMatrix, const char* uniPos) const
{
	shader.setMat4(uniMatrix, m_cameraMatrix);
	shader.setVec3(uniPos, m_position);
}
void Camera::Matrix(ShaderProgram& shader, const char* uniform)
{
	shader.setMat4(uniform, m_cameraMatrix);
}

void Camera::Input(GLFWwindow* window, float dt)
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		m_position += m_speed * m_direction * dt;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		m_position += m_speed * -m_direction * dt;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		m_position += m_speed * -glm::normalize(glm::cross(m_direction, m_up)) * dt;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		m_position += m_speed * glm::normalize(glm::cross(m_direction, m_up)) * dt;

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		m_position += m_speed * m_up * dt;
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		m_position += m_speed * -m_up * dt;

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		m_speed = 2.0f;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
		m_speed = 0.5f;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS && !m_locked && m_firstClickEsc)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		m_locked = true;
		m_firstClickEsc = false;
	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS && m_locked && m_firstClickEsc)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		m_locked = false;
		m_firstClickEsc = false;
	}
	if (m_locked)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		double mouseX;
		double mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);

		float rotX = m_sensitivity * (float)(mouseY - (m_height / 2)) / m_height;
		float rotY = m_sensitivity * (float)(mouseX - (m_width / 2)) / m_width;

		glm::vec3 newDirection = glm::rotate(m_direction, glm::radians(-rotX), glm::normalize(glm::cross(m_direction, m_up)));
		if (!(glm::angle(newDirection, m_up) <= glm::radians(5.0f)) || glm::angle(newDirection, -m_up) <= glm::radians(5.0f))
			m_direction = newDirection;

		m_direction = glm::rotate(m_direction, glm::radians(-rotY), m_up);

		glfwSetCursorPos(window, (m_width / 2), (m_height / 2));
	}
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE)
	{
		m_firstClickEsc = true;
	}

	// FOR PICKING
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && m_firstClickLeft)
	{
		m_firstClickLeft = false;
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		m_firstClickLeft = true;
	}



	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && m_firstClickF)
	{
		m_flashLight = !m_flashLight;
		m_firstClickF = false;
	}
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_RELEASE)
	{
		m_firstClickF = true;
	}
}

// FOR PICKING
glm::vec2 Camera::CursorClickPosition(GLFWwindow* window)
{
	double mouseOnClickX;
	double mouseOnClickY;
	if (!m_locked && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && m_firstClickLeft)
	{
		glfwGetCursorPos(window, &mouseOnClickX, &mouseOnClickY);
	}
	return glm::vec2(float(mouseOnClickX), float(mouseOnClickY));
}
glm::vec2 Camera::CursorPosition(GLFWwindow* window)
{
	double mouseX;
	double mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);
	return glm::vec2(float(mouseX), float(mouseY));
}