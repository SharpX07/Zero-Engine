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
#include "Resource.h"

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

		template<typename T, typename... Args>
		Ref<T> CreateResource(const std::string& path, Args&&... args)
		{
			
			if constexpr (std::is_same_v<T, Model>)
				return CreateAndStoreResource<Model>(path, [this](const std::string& p) { return this->CreateModel(p); });
			else if constexpr (std::is_same_v<T, Shader>)
				return CreateAndStoreResource<Shader>(path, [this](const std::string& p) { return this->CreateShader(p); });
			else if constexpr (std::is_same_v<T, GLTexture>)
				return CreateAndStoreResource<GLTexture>(path, [this](const std::string& p, Args&&... args) { return this->CreateTexture(p, std::forward<Args>(args)...); }, std::forward<Args>(args)...);
			else if constexpr (std::is_same_v<T, Material>)
				return CreateAndStoreResource<Material>(path, [this](const std::string& p) { return CreateRef<Material>(); });
			else
			{
				ZERO_ASSERT(false, "Unsupported resource type");
			}
		}

		//TODO
		template<typename T>
		Ref<T> GetResource(const std::string& name)
		{
			//Ref<T> existingResource;
			//if constexpr (std::is_same_v<T, Model>)
				//existingResource = FindResource<T>(filename, resourceMap);
			//else
				//ZERO_ASSERT(false, "Unsupported resource type");

			//if (existingResource)
				//return existingResource;
			//else
				//ZERO_CORE_LOG_WARN("Resource not found: {}", name);
			return nullptr;
		}

		static void Clear()
		{
			m_Resources.clear();
		}

		const HashTable<std::string, Ref<Resource>>& GetAllResources() const { return m_Resources; }

	private:


		template<typename T, typename F, typename... Args>
		Ref<T> CreateAndStoreResource(const std::string& path, F creator, Args&&...  args)
		{
			std::string filename = std::filesystem::path(path).filename().string();
			auto existingResource = FindResource<T>(filename);
			if (existingResource)
				return existingResource;

			Ref<T> resource = creator(path, std::forward<Args>(args)...);
			Ref<Resource> SaveResource = std::static_pointer_cast<Resource>(resource);
			SaveResource->SetName(filename);

			if (resource) {
				m_Resources.insert({ filename, SaveResource });
			}
			return resource;
		}

		template<typename T>
		Ref<T> FindResource(const std::string& filename)
		{
			auto it = m_Resources.find(filename);
			if (it != m_Resources.end()) return std::static_pointer_cast<T>(it->second);
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
		template<typename... Args>
		Ref<GLTexture> CreateTexture(const std::string& path,Args&&... args)
		{
			return CreateRef<GLTexture>(path.c_str(), std::forward<Args>(args)...);
		}

		
	private:
		static HashTable<std::string, Ref<Resource>> m_Resources;
		ModelImporter m_ModelImporter;
		ShaderParser m_ShaderParser;
	};
}
