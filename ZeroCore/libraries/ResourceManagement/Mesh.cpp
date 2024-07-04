#pragma once
#include "Mesh.h"
#include <ResourceManagement/Material.h>
namespace Zero
{

	Mesh::Mesh(std::vector<mesh::MeshVertex> vertices, std::vector<unsigned int> indices, const Zero::Material& material)
	{
		this->Material = material;
		this->VertexIndices = indices;
		this->Vertices = vertices;
		SetupMesh();
	}

	void Mesh::SetupMesh()
	{
		m_VAO = std::make_shared<Zero::VAO>();
		Zero::VertexAttributeLayout layout;
		m_VAO->Bind();

		m_VBO = std::make_shared<Zero::VBO>(&Vertices[0], Vertices.size() * sizeof(mesh::MeshVertex));
		m_EBO = std::make_shared<Zero::EBO>(&VertexIndices[0], VertexIndices.size() * sizeof(unsigned int));
		layout.addAttribute(GL_FLOAT, 3);
		layout.addAttribute(GL_FLOAT, 3);
		layout.addAttribute(GL_FLOAT, 2);

		m_VAO->AddBuffer(*m_VBO, layout);

	}
};

