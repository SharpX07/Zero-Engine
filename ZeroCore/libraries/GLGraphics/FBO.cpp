#include <GLGraphics/FBO.h>
#include <stdexcept>
#include <Core/Logger.h>
namespace Zero
{
	Framebuffer::Framebuffer()
	{
	}

	Framebuffer::~Framebuffer()
	{
		// Eliminar color attachments
		for (auto attachment : m_ColorAttachments)
		{
			glDeleteTextures(1, &attachment);
		}

		// Eliminar depth attachment (si es una textura)
		if (m_DepthAttachment != 0)
		{
			glDeleteTextures(1, &m_DepthAttachment);
		}

		// Finalmente, eliminar el framebuffer
		if (m_FramebufferId != 0)
		{
			glDeleteFramebuffers(1, &m_FramebufferId);
		}
	}

	void Framebuffer::Create(FramebufferConfiguration configuration)
	{
		m_Width = configuration.Width;
		m_Height = configuration.Height;
		m_Configuration = configuration;
		glGenFramebuffers(1, &m_FramebufferId);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferId);
		AddAttachments();
		GLenum framebufferStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (framebufferStatus != GL_FRAMEBUFFER_COMPLETE)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0); // Desvincula el framebuffer antes de lanzar una excepci�n
			throw std::runtime_error("Framebuffer creation failed. Status: " + GetFramebufferStatusString(framebufferStatus));
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Framebuffer::Bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferId);
	}

	void Framebuffer::UnBind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Framebuffer::RescaleFramebuffer(int width, int height)
	{
		// Verificar si el tamaño ha cambiado realmente
		if (width == m_Width && height == m_Height)
			return;

		m_Width = width;
		m_Height = height;

		// Reescalar color attachments
		for (size_t i = 0; i < m_ColorAttachments.size(); ++i)
		{
			glBindTexture(GL_TEXTURE_2D, m_ColorAttachments[i]);
			FrameBufferFormat fformat = m_Configuration.Formats[i];
			GLenum internalFormat = static_cast<GLenum>(fformat);
			GLenum format, type;
			GetFormatAndType(fformat, format, type);
			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_Width, m_Height, 0,
				format, type, nullptr);
		}

		// Reescalar depth attachment (si es una textura)
		if (m_DepthAttachment != 0)
		{
			glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
			FrameBufferFormat fformat = m_Configuration.Formats.back(); // Asumiendo que el depth está al final
			GLenum internalFormat = static_cast<GLenum>(fformat);
			GLenum format, type;
			GetFormatAndType(fformat, format, type);
			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_Width, m_Height, 0,
				format, type, nullptr);
		}

		// Verificar si el framebuffer está completo después de reescalar
		glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferId);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			// Manejar el error, por ejemplo:
			throw std::runtime_error("Framebuffer is not complete after rescaling!");
		}

		// Desenlazar el framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Framebuffer::AddAttachments()
	{
		GLuint colorAttachmentCount = 0;

		for (auto format : m_Configuration.Formats)
		{
			switch (format)
			{
			case FrameBufferFormat::RGBA8:
			case FrameBufferFormat::RGB8:
			case FrameBufferFormat::RGBA16F:
			case FrameBufferFormat::RGB16F:
			case FrameBufferFormat::RGBA32F:
			case FrameBufferFormat::RGB32F:
			case FrameBufferFormat::R8:
			case FrameBufferFormat::RG8:
			case FrameBufferFormat::R16F:
			case FrameBufferFormat::RG16F:
			case FrameBufferFormat::R32F:
			case FrameBufferFormat::RG32F:
			case FrameBufferFormat::SRGB8:
			case FrameBufferFormat::SRGBA8:
			case FrameBufferFormat::R32I:
			case FrameBufferFormat::RG32I:
			case FrameBufferFormat::RGB32I:
			case FrameBufferFormat::RGBA32I:
			case FrameBufferFormat::R32UI:
			case FrameBufferFormat::RG32UI:
			case FrameBufferFormat::RGB32UI:
			case FrameBufferFormat::RGBA32UI:
			{
				// Color attachment
				GLuint textureID;
				glGenTextures(1, &textureID);
				glBindTexture(GL_TEXTURE_2D, textureID);
				GLenum internalFormat = static_cast<GLenum>(format);
				GLenum outformat, type;
				GetFormatAndType(format, outformat, type);
				glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_Width, m_Height, 0,
					outformat, type, nullptr);
				//glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLenum>(format), m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + colorAttachmentCount, GL_TEXTURE_2D, textureID, 0);

				m_ColorAttachments.push_back(textureID);
				colorAttachmentCount++;
				break;
			}
			case FrameBufferFormat::DEPTH24:
			case FrameBufferFormat::DEPTH32F:
			{
				// Depth attachment
				GLuint depthTextureID;
				glGenTextures(1, &depthTextureID);
				glBindTexture(GL_TEXTURE_2D, depthTextureID);
				glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLenum>(format), m_Width, m_Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTextureID, 0);

				m_DepthAttachment = depthTextureID;
				break;
			}
			}
		}

		if (colorAttachmentCount > 0)
		{
			std::vector<GLenum> attachments(colorAttachmentCount);
			for (GLuint i = 0; i < colorAttachmentCount; i++)
			{
				attachments[i] = GL_COLOR_ATTACHMENT0 + i;
				ZERO_CORE_LOG_DEBUG(i);
			}
			glDrawBuffers(colorAttachmentCount, attachments.data());
		}
		else
		{
			// Solo depth/stencil framebuffer
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
		}
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
	void Framebuffer::GetFormatAndType(FrameBufferFormat internalFormat, GLenum& format, GLenum& type)
	{
		switch (internalFormat)
		{
		case FrameBufferFormat::RGBA8:
		case FrameBufferFormat::SRGBA8:
			format = GL_RGBA;
			type = GL_UNSIGNED_BYTE;
			break;
		case FrameBufferFormat::RGB8:
		case FrameBufferFormat::SRGB8:
			format = GL_RGB;
			type = GL_UNSIGNED_BYTE;
			break;
		case FrameBufferFormat::RGBA16F:
		case FrameBufferFormat::RGBA32F:
			format = GL_RGBA;
			type = GL_FLOAT;
			break;
		case FrameBufferFormat::RGB16F:
		case FrameBufferFormat::RGB32F:
			format = GL_RGB;
			type = GL_FLOAT;
			break;
		case FrameBufferFormat::R8:
			format = GL_RED;
			type = GL_UNSIGNED_BYTE;
			break;
		case FrameBufferFormat::RG8:
			format = GL_RG;
			type = GL_UNSIGNED_BYTE;
			break;
		case FrameBufferFormat::R16F:
		case FrameBufferFormat::R32F:
			format = GL_RED;
			type = GL_FLOAT;
			break;
		case FrameBufferFormat::RG16F:
		case FrameBufferFormat::RG32F:
			format = GL_RG;
			type = GL_FLOAT;
			break;
		case FrameBufferFormat::DEPTH24:
		case FrameBufferFormat::DEPTH32F:
			format = GL_DEPTH_COMPONENT;
			type = GL_FLOAT;
			break;
		case FrameBufferFormat::DEPTH24_STENCIL8:
		case FrameBufferFormat::DEPTH32F_STENCIL8:
			format = GL_DEPTH_STENCIL;
			type = GL_UNSIGNED_INT_24_8;
			break;
		case FrameBufferFormat::STENCIL8:
			format = GL_STENCIL_INDEX;
			type = GL_UNSIGNED_BYTE;
			break;
		case FrameBufferFormat::R32I:
			format = GL_RED_INTEGER;
			type = GL_INT;
			break;
		case FrameBufferFormat::RG32I:
			format = GL_RG_INTEGER;
			type = GL_INT;
			break;
		case FrameBufferFormat::RGB32I:
			format = GL_RGB_INTEGER;
			type = GL_INT;
			break;
		case FrameBufferFormat::RGBA32I:
			format = GL_RGBA_INTEGER;
			type = GL_INT;
			break;
		case FrameBufferFormat::R32UI:
			format = GL_RED_INTEGER;
			type = GL_UNSIGNED_INT;
			break;
		case FrameBufferFormat::RG32UI:
			format = GL_RG_INTEGER;
			type = GL_UNSIGNED_INT;
			break;
		case FrameBufferFormat::RGB32UI:
			format = GL_RGB_INTEGER;
			type = GL_UNSIGNED_INT;
			break;
		case FrameBufferFormat::RGBA32UI:
			format = GL_RGBA_INTEGER;
			type = GL_UNSIGNED_INT;
			break;

		default:
			throw std::runtime_error("Unsupported FrameBufferFormat");
		}
	}
}
