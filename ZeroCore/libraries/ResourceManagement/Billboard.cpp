#include "Billboard.h"
#include <vector>
#include <GLGraphics/EBO.h>
#include <ResourceManagement/ResourceManager.h>
namespace Zero
{
	const std::vector<float> Billboard::s_Vertices = {
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f,   1.0f, 0.0f,
		 0.5f,  0.5f, 0.0f,   1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f,   0.0f, 1.0f
	};

	const std::vector<unsigned int> Billboard::s_Indices = {
		0, 1, 2,
		2, 3, 0
	};

	Ref<Shader> Billboard::s_Shader;
	Billboard::Billboard(Ref<GLTexture> texture)
	{
		m_VAO = CreateScope<VAO>();
		m_VAO->Bind();
		m_VBO = CreateScope<VBO>(&s_Vertices[0], s_Vertices.size() * sizeof(float));
		m_EBO = CreateScope<EBO>(&s_Indices[0], s_Indices.size() * sizeof(unsigned int));
		VertexAttributeLayout layout;
		layout.addAttribute(GL_FLOAT, 3);
		layout.addAttribute(GL_FLOAT, 2);
		m_VAO->AddBuffer(*m_VBO, layout);
		m_Texture = texture;
		s_Shader = ResourceManager::GetInstance().CreateResource<Shader>("Assets/Core/Shaders/BillboardShader.glsl");
	}
}