#pragma once
#include <glad/glad.h>
#include <string>
namespace zeroengine
{
	class Framebuffer
	{
	public:
		Framebuffer(int width, int height);
		~Framebuffer();

		void Bind() const;
		void UnBind() const;

	private:
		void CreateColorAttachment(int width, int height);
		void CreateDepthAttachment(int width, int height);
		std::string GetFramebufferStatusString(GLenum status);

	private:
		GLuint framebufferId_;
		GLuint colorTextureId_;
		GLuint depthRenderbufferId_;
	};
}

