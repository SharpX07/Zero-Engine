#pragma once
#include <glad/glad.h>
#include <string>
#include <vector>
namespace Zero
{
	enum class FrameBufferFormat
	{
		RGBA8 = GL_RGBA8,
		RGB8 = GL_RGB8,
		RGBA16F = GL_RGBA16F,
		RGB16F = GL_RGB16F,
		RGBA32F = GL_RGBA32F,
		RGB32F = GL_RGB32F,
		R8 = GL_R8,
		RG8 = GL_RG8,
		R16F = GL_R16F,
		RG16F = GL_RG16F,
		R32F = GL_R32F,
		RG32F = GL_RG32F,
		DEPTH24 = GL_DEPTH_COMPONENT24,
		DEPTH32F = GL_DEPTH_COMPONENT32F,
		DEPTH24_STENCIL8 = GL_DEPTH24_STENCIL8,
		DEPTH32F_STENCIL8 = GL_DEPTH32F_STENCIL8,
		STENCIL8 = GL_STENCIL_INDEX8,
		SRGB8 = GL_SRGB8,
		SRGBA8 = GL_SRGB8_ALPHA8,
		R32I = GL_R32I,
		RG32I = GL_RG32I,
		RGB32I = GL_RGB32I,
		RGBA32I = GL_RGBA32I,
		// Nuevos formatos para enteros sin signo de 32 bits
		R32UI = GL_R32UI,
		RG32UI = GL_RG32UI,
		RGB32UI = GL_RGB32UI,
		RGBA32UI = GL_RGBA32UI

	};

	struct FramebufferConfiguration
	{
		int Width;
		int Height;
		std::vector<FrameBufferFormat> Formats;
	};

	class Framebuffer
	{
	public:
		Framebuffer();
		~Framebuffer();

		void Create(FramebufferConfiguration configuration);
		void Bind() const;
		void UnBind() const;
		int GetColorTexture(int index) const { return m_ColorAttachments.at(index); }
		void RescaleFramebuffer(int width, int height);
	private:
		void AddAttachments();
		std::string GetFramebufferStatusString(GLenum status);
		static void GetFormatAndType(FrameBufferFormat internalFormat, GLenum& format, GLenum& type);


	private:
		GLuint m_Width;
		GLuint m_Height;
		GLuint m_FramebufferId;
		GLuint m_ColorTextureId;
		GLuint m_DepthRenderbufferId;
		std::vector<GLuint> m_ColorAttachments;
		GLuint m_DepthAttachment;
		FramebufferConfiguration m_Configuration;
	};
}
