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

namespace Zero
{

	namespace mesh
	{
		struct MeshVertex {
			glm::vec3 Position;
			glm::vec2 TexCoords;
		};

		enum MeshTextureTypes {
			DIFFUSE,
			SPECULAR,
			NORMAL,
			ROUGHNESS,
			METALNESS,
			EMISSIVE
		};
		/*struct Texture {
			unsigned int id;
			string type;
			string path;
		};
		struct MeshTexture {
			unsigned int 
		};*/
	}

	class Mesh {
	public:

		Mesh(std::vector<mesh::MeshVertex> vertices, std::vector<unsigned int> indices);
		// render the mesh
		void Draw(const Zero::Shader& shader);
		void SetupMesh();

	private:
		Zero::VBO* m_VBO;
		Zero::EBO* m_EBO;
	public:
		std::vector<mesh::MeshVertex>	vertices;
		std::vector<unsigned int>		indices;
		Zero::VAO*				m_VAO;
	};

}

