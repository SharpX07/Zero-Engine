#pragma once

#include <glad/glad.h>

#include <string>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <ResourceManagement/Resource.h>

namespace Zero
{
	/// <summary>
	/// Work with textures on files and embedded
	/// </summary>
	class GLTexture :public Resource {
	public:
		GLTexture(const char* path = "");
		GLTexture(const char* name, unsigned char* data, int size);
		~GLTexture();
		void SetTexture(const char* texturePath);
		void Bind(GLuint slot) const;
		void Unbind() const;

	private:
		void SetPixelStoreAlignment(int width);
		void DescribeTexture(int m_NrChannels);
		void SetTextureParameters(GLenum wrapS, GLenum wrapT, GLenum minFilter, GLenum magFilter);
		int LoadTextureFromFile(int& width, int& height, int& nrChannels, const char* texturePath);
		int LoadTextureFromMemory(unsigned char* data, int size, int& width, int& height, int& nrChannels);
		GLboolean m_HasTransparency = false;
		int m_Width=0, m_Height=0, m_NrChannels=0;
		unsigned char* m_data;
		GLuint m_ID=0;
	};
}
