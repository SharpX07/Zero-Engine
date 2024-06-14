
#include "ModelReader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>
#include <Core/Logger.h>
namespace Zero
{

	std::shared_ptr<Model> ModelImporter::loadModel(const char* _modelPath)
	{

		Assimp::Importer importer;
		std::shared_ptr<Model> newModel = std::make_shared<Model>();
		const aiScene* scene = importer.ReadFile(_modelPath,
			aiProcess_Triangulate |
			aiProcess_FlipUVs |
			aiProcess_GenSmoothNormals |
			aiProcess_JoinIdenticalVertices);

		if (!scene) {
			ZERO_APP_LOG_ERROR(importer.GetErrorString())
			return NULL;
		}
		std::string convertedPath(_modelPath);
		newModel->SetPath(convertedPath.substr(0, convertedPath.find_last_of("/\\")));

		LoadModelMaterials(scene, newModel);
		ExploreNode(scene->mRootNode, scene, newModel, AssimpToGLM(scene->mRootNode->mTransformation));
		m_MeshTexturesLoaded.clear();
		return newModel;
	}

	void ModelImporter::LoadModelMaterials(const aiScene* scene, std::shared_ptr<Model> model)
	{
		for (int i = 0; i < scene->mNumMaterials; i++)
		{
			aiMaterial* material = scene->mMaterials[i];
			Material newMaterial;
			std::vector<MeshTexture> textures;
			textures = LoadMaterialTextures(material, model);
			for (const auto& texture : textures)
				newMaterial.addTexture(texture);
			model->addMaterial(newMaterial);
		}
	}

	void ModelImporter::ExploreNode(aiNode* node, const aiScene* scene, std::shared_ptr<Model> model, glm::mat4 mTrasformation)
	{
		glm::mat4 matrixTransformation = mTrasformation;
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

			model->addMesh(loadMesh(mesh, scene, model, matrixTransformation));
		}
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{

			ExploreNode(node->mChildren[i], scene, model, matrixTransformation + AssimpToGLM(node->mChildren[i]->mTransformation));
		}
	}

	Mesh ModelImporter::loadMesh(aiMesh* mesh, const aiScene* scene, std::shared_ptr<Model> model, glm::mat4& mat)
	{

		std::vector<mesh::MeshVertex> vertices;
		std::vector<unsigned int> indices;
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			mesh::MeshVertex vertex;
			vertex.Normal = AssimpToGLM(mesh->mNormals[i]);
			glm::mat3 rotation = glm::mat3(mat);
			vertex.Normal = rotation * vertex.Normal;
				vertex.Position = AssimpToGLM(mesh->mVertices[i]);
			vertex.Position = mat * glm::vec4(AssimpToGLM(mesh->mVertices[i]), 1.0f);

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

	std::vector<MeshTexture> ModelImporter::LoadMaterialTextures(aiMaterial* mat, const std::shared_ptr<Model> model)
	{

		std::vector<MeshTexture> textures;
		auto loadTexturesOfType = [&](aiTextureType textureType, MeshTextureTypes meshTextureType) {
			for (unsigned int i = 0; i < mat->GetTextureCount(textureType); i++)
			{
				MeshTexture texture;
				aiString str;
				mat->GetTexture(textureType, i, &str);
				texture.Path = model->GetPath() + "/" + std::string(str.C_Str());
				texture.Type = meshTextureType;
				std::shared_ptr<GLTexture> sameGLTexture = nullptr;
				for (auto const& meshTexture : m_MeshTexturesLoaded)
				{
					if (meshTexture.Path == texture.Path)
					{
						sameGLTexture = meshTexture.GlTexture;
						break;
					}
				}
				if (sameGLTexture)
					texture.GlTexture = sameGLTexture;
				else
				{
					texture.GlTexture = std::make_shared<GLTexture>(texture.Path.c_str());
					m_MeshTexturesLoaded.push_back(texture);
				}
				textures.push_back(texture);
			}
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