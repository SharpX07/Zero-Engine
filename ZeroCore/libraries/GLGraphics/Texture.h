#pragma once

#include <glad/glad.h>

#include <string>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

namespace Zero
{
	class GLTexture {
	public:
		GLTexture(const char* path = "", bool embedded = false, unsigned char* data = nullptr, int size = 0);
		~GLTexture();
		void SetTexture(const char* texturePath);
		void Bind(unsigned int slot) const;
		void Unbind() const;

	private:
		void SetPixelStoreAlignment(int width);
		void SetTextureParameters(GLenum wrapS, GLenum wrapT, GLenum minFilter, GLenum magFilter);
		int LoadTextureFromFile(int& width, int& height, int& nrChannels, const char* texturePath);
		int LoadTextureFromMemory(unsigned char* data, int size, int& width, int& height, int& nrChannels);
		GLboolean hasTransparency = false;
		int m_width, m_height, m_nrChannels;
		unsigned char* m_data;
		unsigned int m_id;
	};
}
