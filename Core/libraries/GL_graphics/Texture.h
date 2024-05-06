#pragma once

#include <glad/glad.h>

#include <string>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm.hpp>

namespace Zero
{
	class GLTexture {
	public:
        GLTexture(const char* texturePath="");
		~GLTexture();
		void SetTexture(const char* texturePath);
		void Bind(unsigned int slot) const;
		void Unbind() const;
    private:
		void SetPixelStoreAlignment(int width);
		void SetTextureParameters(GLenum wrapS, GLenum wrapT, GLenum minFilter, GLenum magFilter);
		int LoadTexture(int& width, int& height, int& nrChannels, const char* texturePath);
        GLboolean hasTransparency=false;

        unsigned int m_id;
        int m_width, m_height, m_nrChannels;
		unsigned char *m_data;
	};
}
