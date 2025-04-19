#pragma once
#include <glad/glad.h> // holds all OpenGL type declarations
#include <string>
#include <vector>
#include "Mesh.h"
#include "Material.h"
#include "Resource.h"
#include <Core/Aliases.h>

namespace Zero
{
	struct BoundingBox
	{
		glm::vec3 min = glm::vec3(std::numeric_limits<float>::max());;
		glm::vec3 max = glm::vec3(std::numeric_limits<float>::lowest());;
	};
	class Model: public Resource {
	public:
		~Model(){}
	public:
		inline void addMaterial(Ref<Material> material) { m_Materials.push_back(material); }
		inline void addMesh(const Mesh& mesh) { m_Meshes.push_back(mesh); }
		inline const std::vector<Mesh>& GetMeshes() const { return m_Meshes; }
		inline const std::vector<Ref<Material>>& GetMaterials() const { return m_Materials; }
		inline const std::string& GetPath() const { return m_Path; }
		inline void SetPath(std::string path) { m_Path=path;}
		inline void SetBoundingBox(BoundingBox bb) { m_BoundingBox = bb; }
		inline BoundingBox GetBoundingBox() { return m_BoundingBox; }
	private:
		std::vector<Mesh>		m_Meshes;
		std::vector<Ref<Material>>	m_Materials;
		std::string				m_Path;
		BoundingBox m_BoundingBox;
	};
}
