#include "Renderer.h"
#include <core/Logger.h>
namespace Zero
{
	void Renderer::Render(Model& model, Shader& shader)
	{
		for (const auto& mesh : model.GetMeshes()) {
			for (unsigned int i = 0; i < mesh.Material.GetNumTextures(); i++)
			{
				const auto& texture = mesh.Material.getTextures().at(i);
				if (texture.Type == DIFFUSE)
					texture.GlTexture->Bind(0);
				if (texture.Type == NORMAL)
					texture.GlTexture->Bind(1);
			}
			mesh.m_VAO->Bind();
			glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(mesh.VertexIndices.size()), GL_UNSIGNED_INT, 0);
			glActiveTexture(GL_TEXTURE0);
		}
	}
	void Renderer::InitializeRenderer()
	{
		noTextureSample = std::make_unique<GLTexture>("Assets/Core/Textures/missing_texture.jpg");
		ZERO_CORE_LOG_INFO("Cargando Textura Predefinida");
	}
}