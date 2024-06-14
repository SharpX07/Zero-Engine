#pragma once

#include <glad/glad.h>

#include <vector>
#include <ResourceManagement/Mesh.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <glm/glm.hpp>
#include <ResourceManagement/Model.h>
namespace Zero
{
	class ModelImporter {
	public:
		ModelImporter() = default;
		~ModelImporter()
		{
		}
		std::shared_ptr<Model> loadModel(const char* _modelPath);
	private:
		void ExploreNode(aiNode* node, const aiScene* scene, std::shared_ptr<Model> model, glm::mat4 mTrasformation);
		std::vector<MeshTexture> LoadMaterialTextures(aiMaterial* mat, const std::shared_ptr<Model> model);
		void LoadModelMaterials(const aiScene* scene, std::shared_ptr<Model> model);
		Mesh loadMesh(aiMesh* mesh, const aiScene* scene, std::shared_ptr<Model> model, glm::mat4& mat);
		glm::vec3 AssimpToGLM(aiVector3D vector);
		glm::mat4 AssimpToGLM(aiMatrix4x4& mat);
		std::vector<MeshTexture> m_MeshTexturesLoaded;
	};
}
