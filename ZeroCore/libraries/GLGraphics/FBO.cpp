#include <GLGraphics/FBO.h>
#include <stdexcept>
namespace Zero
{
	Framebuffer::Framebuffer(int width, int height)
	{
		glGenFramebuffers(1, &m_FramebufferId);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferId);

		CreateColorAttachment(width, height);
		CreateDepthAttachment(width, height);

		GLenum framebufferStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);

		if (framebufferStatus != GL_FRAMEBUFFER_COMPLETE)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0); // Desvincula el framebuffer antes de lanzar una excepci�n
			throw std::runtime_error("Framebuffer creation failed. Status: " + GetFramebufferStatusString(framebufferStatus));
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	Framebuffer::~Framebuffer()
	{
		glDeleteFramebuffers(1, &m_FramebufferId);
		glDeleteTextures(1, &m_ColorTextureId);
		glDeleteRenderbuffers(1, &m_DepthRenderbufferId);
	}

	void Framebuffer::Bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferId);
	}

	void Framebuffer::UnBind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Framebuffer::RescaleFramebuffer(float width, float height)
	{
		glBindTexture(GL_TEXTURE_2D, m_ColorTextureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorTextureId, 0);

		glBindRenderbuffer(GL_RENDERBUFFER, m_DepthRenderbufferId);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthRenderbufferId);
	}

	void Framebuffer::CreateColorAttachment(int width, int height)
	{
		glGenTextures(1, &m_ColorTextureId);
		glBindTexture(GL_TEXTURE_2D, m_ColorTextureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorTextureId, 0);
	}

	void Framebuffer::CreateDepthAttachment(int width, int height)
	{
		glGenRenderbuffers(1, &m_DepthRenderbufferId);
		glBindRenderbuffer(GL_RENDERBUFFER, m_DepthRenderbufferId);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthRenderbufferId);
	}
	std::string Framebuffer::GetFramebufferStatusString(GLenum status)
	{
		switch (status)
		{
		case GL_FRAMEBUFFER_COMPLETE:
			return "Framebuffer is complete.";
		case GL_FRAMEBUFFER_UNDEFINED:
			return "Framebuffer undefined.";
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			return "Framebuffer incomplete attachment.";
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			return "Framebuffer incomplete missing attachment.";
		case GL_FRAMEBUFFER_UNSUPPORTED:
			return "Framebuffer unsupported.";
		default:
			return "Unknown framebuffer status.";
		}
	}
}
