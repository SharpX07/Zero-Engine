#pragma once
#include <glad/glad.h> // holds all OpenGL type declarations

#include <string>
#include <vector>

#include <GLGraphics/AttributeLayout.h>
#include <GLGraphics/VAO.h>
#include <GLGraphics/VBO.h>
#include <GLGraphics/EBO.h>
#include <GLGraphics/Shader.h>
#include <GLGraphics/Texture.h>
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

