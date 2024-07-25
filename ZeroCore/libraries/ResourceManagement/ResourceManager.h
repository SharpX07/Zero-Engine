#pragma once
#include <Core/Aliases.h>
#include <unordered_map>
#include <ResourceManagement/model.h>
#include <GLGraphics/ShaderParser.h>
#include <Modules/ModelReader.h>
#include <iostream>
#include <filesystem>
#include <Core/Logger.h>
#include <Core/Assert.h>
#include <type_traits>

namespace Zero
{
	class ResourceManager
	{
	public:
		static ResourceManager& GetInstance()
		{
			static ResourceManager instance;
			return instance;
		}

		template<typename T>
		Ref<T> CreateResource(const std::string& path)
		{
			if constexpr (std::is_same_v<T, Model>)
			{
				return CreateAndStoreResource<Model>(path, m_ModelResources, [this](const std::string& p) { return this->CreateModel(p); });
			}
			else if constexpr (std::is_same_v<T, Shader>)
			{
				return CreateAndStoreResource<Shader>(path, m_ShaderResources, [this](const std::string& p) { return this->CreateShader(p); });
			}
			else
			{
				ZERO_ASSERT(false, "Unsupported resource type");
			}
		}

		template<typename T>
		Ref<T> GetResource(const std::string& name)
		{
			Ref<T> existingResource;
			if constexpr (std::is_same_v<T, Model>)
				existingResource = FindResource<T>(filename, resourceMap);
			else
				ZERO_ASSERT(false, "Unsupported resource type");

			if (existingResource)
				return existingResource;
			else
				ZERO_CORE_LOG_WARN("Resource not found: {}", name);
			return nullptr;
		}

		static void Clear()
		{
			m_ModelResources.clear();
			m_TextureResources.clear();
			m_ShaderResources.clear();
		}

	private:
		

		template<typename T, typename F>
		Ref<T> CreateAndStoreResource(const std::string& path, HashTable<std::string, Ref<T>>& resourceMap, F creator)
		{
			std::string filename = std::filesystem::path(path).filename().string();

			Ref<T> existingResource = FindResource<T>(filename, resourceMap);
			if (existingResource) return existingResource;

			Ref<T> resource = creator(path);
			if (resource) resourceMap.insert({ filename, resource });
			return resource;
		}

		template<typename T>
		Ref<T> FindResource(const std::string& filename, const HashTable<std::string, Ref<T>>& resourceMap)
		{
			auto it = resourceMap.find(filename);
			if (it != resourceMap.end()) return it->second;
			return nullptr;
		}

		Ref<Model> CreateModel(const std::string& path)
		{
			return m_ModelImporter.loadModel(path.c_str());
		}

		Ref<Shader> CreateShader(const std::string& path)
		{
			return m_ShaderParser.GenerateShader(path.c_str());
		}


	private:
		static HashTable<std::string, Ref<Model>> m_ModelResources;
		static HashTable<std::string, Ref<GLTexture>> m_TextureResources;
		static HashTable<std::string, Ref<Shader>> m_ShaderResources;
		ModelImporter m_ModelImporter;
		ShaderParser m_ShaderParser;
	};
}