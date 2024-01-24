#include "PickFBO.h"

PickFBO::PickFBO(GLuint width, GLuint height)
{
	Init(width, height);
}

PickFBO::~PickFBO()
{
	if (m_fbo != 0)
		glDeleteFramebuffers(1, &m_fbo);
	if (m_texture != 0)
		glDeleteTextures(1, &m_texture);
}

void PickFBO::Init(GLuint width, GLuint height)
{
	GenFramebuffer();
	Bind();
	SetWidth(width);
	SetHeight(height);
	GenTexture();
	AttachRGB32UI();
	GenRBO();
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cerr << "Framebuffer not complete: " << status << FramebufferStatusEnum(status) << std::endl;
		exit(1);
	}
	Unbind();
}

bool PickFBO::Clicked(GLFWwindow* window, Camera& camera)
{
	if (!camera.IsLocked() && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS
		&& camera.IsFirstClickLeft())
		return true;
	else
		return false;
}
glm::vec3 PickFBO::UpdateObjectPosition(GLFWwindow* window, glm::vec3 crntObjPosition, glm::vec2 cursor, Camera camera)
{
	glm::mat4 projectionInv = glm::inverse(camera.GetProjection());
	glm::mat4 view = camera.GetView();
	glm::mat4 viewInv = glm::inverse(view);

	// Normalized Device Coordinates [-1, 1] , assuming window and camera the same size
	float height = float(camera.GetHeight());
	float width = float(camera.GetWidth());
	float x_ndc = 2.0f * cursor.x / width - 1.0f;
	float y_ndc = -(2.0f * cursor.y / height - 1.0f);
	float focal_length = 1.0f / tanf(glm::radians(camera.GetFOVdeg() / 2.0f));
	float aspectRatio = float(width / height);

	// ray in view space
	glm::vec3 ray_view(-x_ndc / focal_length * aspectRatio, -y_ndc / focal_length, 1.0f);

	// intersect ray with object z-plane (view space)
	float z_obj_view = (view * glm::vec4(crntObjPosition, 1.0)).z;
	glm::vec4 ray_intersect_view(ray_view * z_obj_view, 1.0f);

	// view to world space
	glm::vec4 ray_intersect_world = viewInv * ray_intersect_view;
	return glm::vec3(ray_intersect_world);
}

PickFBO::PixelInfo PickFBO::ReadPixel(GLuint x, GLuint y)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);
	glReadBuffer(GL_COLOR_ATTACHMENT0);
	PixelInfo pixel;
	glReadPixels(x, y, 1, 1, GL_RGB_INTEGER, GL_UNSIGNED_INT, &pixel);
	glReadBuffer(GL_NONE);
	return pixel;
}