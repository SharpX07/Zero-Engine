#include <Buffers/FBO.h>
#include <Debug/Logger.h>
namespace zeroengine
{
	Framebuffer::Framebuffer(int width, int height)
	{
		Logger log;

		glGenFramebuffers(1, &framebufferId_);
		glBindFramebuffer(GL_FRAMEBUFFER, framebufferId_);

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
		glDeleteFramebuffers(1, &framebufferId_);
		glDeleteTextures(1, &colorTextureId_);
		glDeleteRenderbuffers(1, &depthRenderbufferId_);
	}

	void Framebuffer::Bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, framebufferId_);
	}

	void Framebuffer::UnBind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Framebuffer::CreateColorAttachment(int width, int height)
	{
		glGenTextures(1, &colorTextureId_);
		glBindTexture(GL_TEXTURE_2D, colorTextureId_);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTextureId_, 0);
	}

	void Framebuffer::CreateDepthAttachment(int width, int height)
	{
		glGenRenderbuffers(1, &depthRenderbufferId_);
		glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbufferId_);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbufferId_);
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
