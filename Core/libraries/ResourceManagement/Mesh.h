#pragma once
#include <glad/glad.h> // holds all OpenGL type declarations

#include <string>
#include <vector>

#include <Buffers/AttributeLayout.h>
#include <Buffers/VAO.h>
#include <Buffers/VBO.h>
#include <Buffers/EBO.h>
#include <GL_graphics/Shader.h>
#include <GL_graphics/Texture.h>
#include <ResourceManagement/Material.h>
namespace Zero
{

	namespace mesh
	{
		struct MeshVertex {
			glm::vec3 Position;
			glm::vec3 Normal;
			glm::vec2 TexCoords;
		};
	}

	class Mesh {
	public:

		Mesh(
			std::vector<mesh::MeshVertex> vertices,
			std::vector<unsigned int> indices,
			const Material& material
		);
		~Mesh()
		{
			Vertices.clear();
			VertexIndices.clear();
		}

		void SetupMesh();

	private:
		std::shared_ptr<Zero::VBO> m_VBO;
		std::shared_ptr<Zero::EBO> m_EBO;
	public:
		std::vector<mesh::MeshVertex>	Vertices;
		std::vector<unsigned int>		VertexIndices;
		Material Material;
		std::shared_ptr<Zero::VAO> m_VAO;
	};

}

