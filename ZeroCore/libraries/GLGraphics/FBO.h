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
		SRGBA8 = GL_SRGB8_ALPHA8
	};

	struct FramebufferConfiguration
	{
		int Width;
		int Height;
		FrameBufferFormat Formats[3] = {};
	};

	class Framebuffer
	{
	public:
		Framebuffer(int width, int height);
		~Framebuffer();

		void Bind() const;
		void UnBind() const;
		int GetColorTexture() const { return m_ColorTextureId; }
		void RescaleFramebuffer(float width, float height);
	private:
		void CreateColorAttachment(int width, int height);
		void CreateDepthAttachment(int width, int height);
		std::string GetFramebufferStatusString(GLenum status);

	private:
		GLuint m_FramebufferId;
		GLuint m_ColorTextureId;
		GLuint m_DepthRenderbufferId;
	};
}

//{
//	FramebufferConfiguration spec;
//	spec.width = 500;
//	spec.height = 500;
//	spec.Attachments = {at1,at2,at3,at4..};
//	Framebuffer buffer;
//	buffer.AtachColor();
//}
