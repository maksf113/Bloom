#pragma once
#include "FBO.h"
#include "Camera.h"

class Cubemap : public FBO
{
private:
	glm::vec3 m_position;
public:
	Camera m_camera;
	float m_nearPlane = 0.1f;
	float m_farPlane = 100.0f;
	Cubemap(GLenum texType, GLuint size, glm::vec3 position) : m_camera(size, size, position)
	{
		GenFramebuffer();
		SetWidth(size);
		SetHeight(size);
		SetPosition(position);
		SetTexType(texType);
		GenTexture();
		if (!AttachFaces(m_texType))
			std::cerr << "Cubemap error: faces not attached" << std::endl;
	}

	bool AttachFaces(GLenum attachmentType)
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
		GLenum format = GL_RGB; // default GL_COLOR_ATTACHMENT0
		if (attachmentType == GL_DEPTH_ATTACHMENT)
			format = GL_DEPTH_COMPONENT;

		for (GLuint i = 0; i < 6; i++)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, m_width, m_height, 0,
				format, GL_FLOAT, 0);
		SetTexParameter(GL_LINEAR, GL_CLAMP_TO_EDGE);
		Bind();

		glFramebufferTexture(GL_FRAMEBUFFER, attachmentType, m_texture, 0);
		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cerr << "Framebuffer not complete: " << status << FramebufferStatusEnum(status) << std::endl;
			std::cerr << "Attachment type: " << attachmentType << AttachmentEnum(attachmentType) << std::endl;
			return false;
		}
		return true;
	}
	void SetTexParameter(GLenum scaling, GLenum fitting)
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
		// scaling
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, scaling);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, scaling);

		// fitting
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, fitting);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, fitting);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, fitting);
	}
	void SetFaceToDraw(int faceIndex)
	{
		SwitchToFace(faceIndex);
		Bind();
		Viewport();

		glFramebufferTexture(GL_FRAMEBUFFER, m_texType, this->GetTexID(), 0);
	}
	void SwitchToFace(int faceIndex)
	{
		switch (faceIndex)
		{
		case 0:
			m_camera.SetDir(glm::vec3(1.0f, 0.0f, 0.0f));
			m_camera.SetUp(glm::vec3(0.0f, -1.0f, 0.0f));
			break;
		case 1:
			m_camera.SetDir(glm::vec3(-1.0f, 0.0f, 0.0f));
			m_camera.SetUp(glm::vec3(0.0f, -1.0f, 0.0f));
			break;
		case 2:
			m_camera.SetDir(glm::vec3(0.0f, 1.0f, 0.0f));
			m_camera.SetUp(glm::vec3(0.0f, 0.0f, 1.0f));
			break;
		case 3:
			m_camera.SetDir(glm::vec3(0.0f, -1.0f, 0.0f));
			m_camera.SetUp(glm::vec3(0.0f, 0.0f, -1.0f));
			break;
		case 4:
			m_camera.SetDir(glm::vec3(0.0f, 0.0f, 1.0f));
			m_camera.SetUp(glm::vec3(0.0f, -1.0f, 0.0f));
			break;
		case 5:
			m_camera.SetDir(glm::vec3(0.0f, 0.0f, -1.0f));
			m_camera.SetUp(glm::vec3(0.0f, -1.0f, 0.0f));
			break;
		}
		m_camera.updateMatrix(90.0f, m_nearPlane, m_farPlane);
	}
	
	void SetFaceToDraw(Camera& camera, int faceIndex)
	{
		SwitchToFace(camera, faceIndex);
		Bind();
		Viewport();

		glFramebufferTexture(GL_FRAMEBUFFER, m_texType, this->GetTexID(), 0);
	}
	void SwitchToFace(Camera& camera, int faceIndex)
	{
		switch (faceIndex)
		{
		case 0:
			camera.SetDir(glm::vec3(1.0f, 0.0f, 0.0f));
			camera.SetUp(glm::vec3(0.0f, -1.0f, 0.0f));
			break;
		case 1:
			camera.SetDir(glm::vec3(-1.0f, 0.0f, 0.0f));
			camera.SetUp(glm::vec3(0.0f, -1.0f, 0.0f));
			break;
		case 2:
			camera.SetDir(glm::vec3(0.0f, 1.0f, 0.0f));
			camera.SetUp(glm::vec3(0.0f, 0.0f, 1.0f));
			break;
		case 3:
			camera.SetDir(glm::vec3(0.0f, -1.0f, 0.0f));
			camera.SetUp(glm::vec3(0.0f, 0.0f, -1.0f));
			break;
		case 4:
			camera.SetDir(glm::vec3(0.0f, 0.0f, 1.0f));
			camera.SetUp(glm::vec3(0.0f, -1.0f, 0.0f));
			break;
		case 5:
			camera.SetDir(glm::vec3(0.0f, 0.0f, -1.0f));
			camera.SetUp(glm::vec3(0.0f, -1.0f, 0.0f));
			break;
		}
	}

	const Camera& GetCamera() const { return m_camera; }

	void SetPosition(glm::vec3 pos) 
	{ 
		m_position = pos; 
		m_camera.SetPos(pos);
	}
};
