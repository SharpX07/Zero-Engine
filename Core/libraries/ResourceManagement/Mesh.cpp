#pragma once
#include "Mesh.h"
namespace Zero
{

	Mesh::Mesh
	(
		std::vector<mesh::MeshVertex> vertices,
		std::vector<unsigned int> indices,
		std::vector<mesh::MeshTexture> textures
		)
	{
		this->Vertices = vertices;
		this->Indices = indices;
		this->Texures = textures;
		SetupMesh();
	}

	void Mesh::SetupMesh()
	{
		m_VAO = new Zero::VAO();
		Zero::VertexAttributeLayout layout;
		m_VAO->Bind();

		m_VBO = new Zero::VBO(&Vertices[0], Vertices.size() * sizeof(mesh::MeshVertex));
		m_EBO = new Zero::EBO(&Indices[0], Indices.size() * sizeof(unsigned int));
		layout.addAttribute(GL_FLOAT, 3);
		layout.addAttribute(GL_FLOAT, 2);

		m_VAO->AddBuffer(*m_VBO, layout);
		
	}
};

