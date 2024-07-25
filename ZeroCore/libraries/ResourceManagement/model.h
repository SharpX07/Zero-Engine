#pragma once
#include <glad/glad.h> // holds all OpenGL type declarations
#include <string>
#include <vector>
#include <ResourceManagement/Mesh.h>
#include <ResourceManagement/Material.h>

namespace Zero
{
	class Model {
	public:
		~Model()
		{
			m_Meshes.clear();
			m_Materials.clear();
		}
	public:
		inline void addMaterial(const Material& material) { m_Materials.push_back(material); }
		inline void addMesh(const Mesh& mesh) { m_Meshes.push_back(mesh); }
		inline const std::vector<Mesh>& GetMeshes() const { return m_Meshes; }
		inline const std::vector<Material>& GetMaterials() const { return m_Materials; }
		inline const std::string& GetPath() const { return m_Path; }
		inline void SetPath(std::string path) { m_Path=path;}
	private:
		std::vector<Mesh>		m_Meshes;
		std::vector<Material>	m_Materials;
		std::string				m_Path;
	};
}
