#include "Renderer.h"

namespace Zero
{
	void Renderer::Render(Mesh& mesh)
	{
		for (int i = 0; i < mesh.Texures.size(); i++)
		{
			mesh.Texures.at(i).GlTexture->Bind(i);
		}
		mesh.m_VAO->Bind();
		glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(mesh.Indices.size()), GL_UNSIGNED_INT, 0);
		glActiveTexture(GL_TEXTURE0);
	}
}