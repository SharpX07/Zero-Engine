#include "Texture.h"
#include <Debug/Logger.h>
#include <stb_image.h>
#include <GLFW/glfw3.h>

namespace Zero
{
	GLTexture::GLTexture(const char* texturePath)
	{
		if (LoadTexture(m_width, m_height, m_nrChannels, texturePath) != 0)
		{
			Logger logger;
			//logger.logWarning("Texture missing on " + std::string(texturePath));
			LoadTexture(m_width, m_height, m_nrChannels, "Core/Assets/Textures/missing_texture.jpg");
		}
		glGenTextures(1, &m_id);
		glBindTexture(GL_TEXTURE_2D, m_id);
		SetPixelStoreAlignment(m_width);
		SetTextureParameters(GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR);
		if(m_nrChannels==4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA,
				GL_UNSIGNED_BYTE, m_data);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, m_width, m_height, 0, GL_RGB,
				GL_UNSIGNED_BYTE, m_data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	GLTexture::~GLTexture()
	{
		stbi_image_free(m_data);
		glDeleteTextures(1, &m_id);
	}

	void GLTexture::Bind(unsigned int slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_id);
	}

	void GLTexture::Unbind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void GLTexture::SetTexture(const char* texturePath)
	{
		if (m_data)
			stbi_image_free(m_data);
		if (LoadTexture(m_width, m_height, m_nrChannels, texturePath) != 0)
			// Work in progress
			glBindTexture(GL_TEXTURE_2D, m_id);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GL_RGB, GL_UNSIGNED_BYTE, m_data);
	}
	int GLTexture::LoadTexture(int& width, int& height, int& nrChannels, const char* texturePath)
	{
		m_data = stbi_load(texturePath, &width, &height, &nrChannels, 0);
		if (!m_data)
		{
			return -1;
		}
		return 0;
	}

	void GLTexture::SetPixelStoreAlignment(int width)
	{
		if (width % 4 == 0)
			glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		else
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	}

	void GLTexture::SetTextureParameters(GLenum wrapS, GLenum wrapT, GLenum minFilter, GLenum magFilter)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
	}
}