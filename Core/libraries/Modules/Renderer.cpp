#include "Renderer.h"

namespace Zero
{
	void Renderer::Render(Mesh& mesh)
	{
		Zero::GLTexture texture("");
		texture.Bind(0);

		mesh.m_VAO->Bind();
		glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(mesh.indices.size()), GL_UNSIGNED_INT, 0);
		glActiveTexture(GL_TEXTURE0);
	}
}