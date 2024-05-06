#pragma once
#include "Mesh.h"
namespace Zero
{

	Mesh::Mesh(std::vector<mesh::MeshVertex> vertices, std::vector<unsigned int> indices)
	{
		this->vertices = vertices;
		this->indices = indices;
		SetupMesh();
	}

	void Mesh::Draw(const Zero::Shader& shader)
	{
		Zero::GLTexture texture("");
		texture.Bind(0);

		m_VAO->Bind();
		glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
		glActiveTexture(GL_TEXTURE0);
	}

	void Mesh::SetupMesh()
	{
		m_VAO = new Zero::VAO();
		m_VAO->Bind();
		m_VBO = new Zero::VBO(&vertices[0], vertices.size() * sizeof(mesh::MeshVertex));
		Zero::VertexAttributeLayout layout;
		m_EBO = new Zero::EBO(&indices[0], indices.size() * sizeof(unsigned int));
		layout.addAttribute(GL_FLOAT, 3);
		layout.addAttribute(GL_FLOAT, 2);

		m_VAO->AddBuffer(*m_VBO, layout);
	}
};

