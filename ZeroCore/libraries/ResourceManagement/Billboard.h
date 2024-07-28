#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLGraphics/Texture.h>
#include <GLGraphics/VAO.h>
#include <GLGraphics/VBO.h>
#include <GLGraphics/EBO.h>
#include <Core/Aliases.h>
#include <GLGraphics/Shader.h>

namespace Zero
{
	class Billboard
	{
	public:
		Billboard(Ref<GLTexture> texture);
		~Billboard() = default;
		inline const void Bind() const
		{
			m_Texture->Bind(0); 
			m_VAO->Bind();
		}
		inline const Ref<Shader> GetShader() { return s_Shader; }
	private:

		Ref<GLTexture> m_Texture;
		Scope<VAO> m_VAO;
		Scope<VBO> m_VBO;
	    Scope<EBO> m_EBO;

		static const std::vector<float> s_Vertices;
		static const std::vector<unsigned int> s_Indices;
		static Ref<Shader> s_Shader;
	};
}
