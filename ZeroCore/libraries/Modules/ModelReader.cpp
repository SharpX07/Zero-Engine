
#include "ModelReader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>
#include <Core/Logger.h>
#include <Core/UUID.h>
#include <ResourceManagement/ResourceManager.h>
namespace Zero
{
	Ref<Model> ModelImporter::loadModel(const char* _modelPath)
	{

		Assimp::Importer importer;
		Ref<Model> newModel = CreateRef<Model>();
		const aiScene* scene = importer.ReadFile(_modelPath,
			aiProcess_Triangulate |
			aiProcess_FlipUVs |
			aiProcess_JoinIdenticalVertices |
			aiProcess_PreTransformVertices |
			aiProcess_GenBoundingBoxes);

		if (!scene) {
			ZERO_APP_LOG_ERROR(importer.GetErrorString());
			return nullptr;
		}
		newModel->SetPath(_modelPath);
		LoadModelMaterials(scene, newModel);
		ExploreNode(scene->mRootNode, scene, newModel, AssimpToGLM(scene->mRootNode->mTransformation));
		return newModel;
	}

	void ModelImporter::LoadModelMaterials(const aiScene* scene, Ref<Model> model)
	{
		for (int i = 0; i < scene->mNumMaterials; i++)
		{
			aiMaterial* material = scene->mMaterials[i];
			Material newMaterial;
			std::vector<MeshTexture> textures;
			textures = LoadMaterialTextures(material, scene, model);
			for (auto tex : textures)
			{
				if (tex.Type == MeshTextureTypes::DIFFUSE)
				{
					auto& properties = newMaterial.GetProperties();
					properties.hasAlbedoTexture = true;
				}
			}
			for (const auto& texture : textures)
				newMaterial.addTexture(texture);
			auto& properties = newMaterial.GetProperties();

			aiColor3D diffuse;
			if (material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse) == AI_SUCCESS) {

				properties.Diffuse = glm::vec3(diffuse.r, diffuse.g, diffuse.b);
			}

			// Metallic
			float metallic;
			if (material->Get(AI_MATKEY_METALLIC_FACTOR, metallic) == AI_SUCCESS) {
				properties.Metallic = metallic;
			}
			else {
				// Si no se encuentra el factor metálico, intentamos con el color especular
				aiColor3D specular;
				if (material->Get(AI_MATKEY_COLOR_SPECULAR, specular) == AI_SUCCESS) {
					properties.Metallic = (specular.r + specular.g + specular.b) / 3.0f;
				}
			}

			// Roughness
			float roughness;
			if (material->Get(AI_MATKEY_ROUGHNESS_FACTOR, roughness) == AI_SUCCESS) {
				properties.Roughness = roughness;
			}
			else {
				// Si no se encuentra el factor de rugosidad, intentamos con el brillo
				float shininess;
				if (material->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS) {
					// Convertir brillo a rugosidad (esta es una aproximación simple)
					properties.Roughness = 1.0f - std::sqrt(shininess / 100.0f);
				}
			}
			// Emissive
			aiColor3D emissive;
			if (material->Get(AI_MATKEY_COLOR_EMISSIVE, emissive) == AI_SUCCESS)
				properties.Emissive = glm::vec3(emissive.r, emissive.g, emissive.b);

			// Opacity
			float opacity;
			if (material->Get(AI_MATKEY_OPACITY, opacity) == AI_SUCCESS)
				properties.Opacity = opacity;
			model->addMaterial(newMaterial);
		}
	}

	void ModelImporter::ExploreNode(aiNode* node, const aiScene* scene, Ref<Model> model, glm::mat4 mTrasformation)
	{
		glm::mat4 matrixTransformation = mTrasformation;
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			aiAABB meshBB = mesh->mAABB;
			glm::vec3 minBB = AssimpToGLM(meshBB.mMin);
			glm::vec3 maxBB = AssimpToGLM(meshBB.mMax);
			// Actualizar el bounding box del modelo
			BoundingBox bb;
			bb.min = glm::min(model->GetBoundingBox().min, minBB);
			bb.max = glm::max(model->GetBoundingBox().max, maxBB);
			model->SetBoundingBox(bb);
			model->addMesh(loadMesh(mesh, scene, model, matrixTransformation));
		}
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			ExploreNode(node->mChildren[i], scene, model, matrixTransformation + AssimpToGLM(node->mChildren[i]->mTransformation));
		}
	}

	Mesh ModelImporter::loadMesh(aiMesh* mesh, const aiScene* scene, Ref<Model> model, glm::mat4& mat)
	{
		std::vector<mesh::MeshVertex> vertices;
		std::vector<unsigned int> indices;
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			mesh::MeshVertex vertex;
			glm::mat3 rotation = glm::mat3(mat);
			if (mesh->HasNormals())
			{
				vertex.Normal = AssimpToGLM(mesh->mNormals[i]);
				//vertex.Normal = rotation * vertex.Normal;
			}
			vertex.Position = AssimpToGLM(mesh->mVertices[i]);
			//vertex.Position = mat * glm::vec4(AssimpToGLM(mesh->mVertices[i]), 1.0f);
			if (mesh->mTextureCoords[0])
			{
				glm::vec2 vec;
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoords = vec;
			}
			else
				vertex.TexCoords = glm::vec2(0.0f, 0.0f);
			vertices.push_back(vertex);
		}
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}
		return Mesh(vertices, indices, model->GetMaterials().at(mesh->mMaterialIndex));
	}

	std::vector<MeshTexture> ModelImporter::LoadMaterialTextures(aiMaterial* mat, const aiScene* scene, const Ref<Model> model)
	{
		std::vector<MeshTexture> textures;
		auto loadTexturesOfType = [&](aiTextureType textureType, MeshTextureTypes meshTextureType) {
			for (unsigned int i = 0; i < mat->GetTextureCount(textureType); i++)
			{
				MeshTexture texture;
				aiString texturePath;
				std::string convertedPath(model->GetPath());
				texture.Path = convertedPath.substr(0, convertedPath.find_last_of("/\\")) + "/" + std::string(texturePath.C_Str());
				texture.Type = meshTextureType;
				bool hasTexture = mat->GetTexture(textureType, i, &texturePath);
				auto embeddedTexture = scene->GetEmbeddedTexture(texturePath.C_Str());
				if (embeddedTexture)
				{
					std::string embeddedTextureIndex(texturePath.C_Str());
					if (!embeddedTextureIndex.empty() && embeddedTextureIndex[0] == '*')
						embeddedTextureIndex = embeddedTextureIndex.substr(1);
					texture.GlTexture = ResourceManager::GetInstance().CreateResource<GLTexture>(model->GetPath() + embeddedTextureIndex, reinterpret_cast<unsigned char*>(embeddedTexture->pcData), embeddedTexture->mWidth);
				}
				else if (hasTexture)
					texture.GlTexture = ResourceManager::GetInstance().CreateResource<GLTexture>(texture.Path.c_str());
				texture.Identifier = UUID();
				textures.push_back(texture);
			}
			return mat->GetTextureCount(textureType);
			};

		// Cargar texturas de tipo DIFFUSE
		loadTexturesOfType(aiTextureType_DIFFUSE, MeshTextureTypes::DIFFUSE);
		// Cargar texturas de tipo NORMALS
		loadTexturesOfType(aiTextureType_NORMALS, MeshTextureTypes::NORMAL);
		return textures;
	}

	glm::vec3 ModelImporter::AssimpToGLM(aiVector3D vector)
	{
		return glm::vec3(vector.x, vector.y, vector.z);
	}

	glm::mat4 ModelImporter::AssimpToGLM(aiMatrix4x4& from)
	{

		glm::mat4 to;
		to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
		to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
		to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
		to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
		return to;
	}
}