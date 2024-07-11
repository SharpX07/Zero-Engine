#pragma once
#include <Core/Aliases.h>
#include <unordered_map>
#include <ResourceManagement/model.h>
#include <Modules/ModelReader.h>
#include <iostream>
#include <filesystem>
#include <Core/Logger.h>
#include <Core/Assert.h>
#include <type_traits>

namespace Zero
{
	struct Resource
	{

		std::string Name;
		Ref<Model> ModelResource;
		Ref<GLTexture> TextureResource;
	};


	class ResourceManager
	{
	public:
        template<typename T>
        Ref<T> CreateResource(const std::string& path)
        {
            if constexpr (std::is_same_v<T, Model>)
            {
                std::string filename = std::filesystem::path(path).filename().string();
                auto it = m_ModelResources.find(filename);
                if (it != m_ModelResources.end())
                {
                    return std::static_pointer_cast<T>(it->second);
                }

                Ref<Model> resource = CreateModel(path);
                ZERO_APP_LOG_INFO(resource.use_count());
                m_ModelResources.insert({ filename, resource });
                return std::static_pointer_cast<T>(resource);
            }
            else if constexpr (std::is_same_v<T, GLTexture>)
            {
                std::string filename = std::filesystem::path(path).filename().string();
                auto it = m_TextureResources.find(filename);
                if (it != m_TextureResources.end())
                {
                    return std::static_pointer_cast<T>(it->second);
                }

                Ref<GLTexture> resource = CreateTexture(path);
                m_TextureResources.insert({ filename, resource });
                return std::static_pointer_cast<T>(resource);
            }
            else
            {
                static_assert(always_false<T>::value, "Unsupported resource type");
            }
        }

        Ref<Resource> GetResource(const std::string& name)
        {
            auto resource = CreateRef<Resource>();
            resource->Name = name;

            auto modelIt = m_ModelResources.find(name);
            if (modelIt != m_ModelResources.end())
            {
                resource->ModelResource = modelIt->second;
            }

            auto textureIt = m_TextureResources.find(name);
            if (textureIt != m_TextureResources.end())
            {
                resource->TextureResource = textureIt->second;
            }

            if (!resource->ModelResource && !resource->TextureResource)
            {
                ZERO_CORE_LOG_WARN("Resource not found: {}", name);
                return nullptr;
            }

            return resource;
        }

        void Clear()
        {
            m_ModelResources.clear();
            m_TextureResources.clear();
        }

    private:
        Ref<Model> CreateModel(const std::string& path)
        {
            try
            {
                return m_ModelImporter.loadModel(path.c_str());
            }
            catch (const std::exception& e)
            {
                ZERO_CORE_LOG_ERROR("Failed to load model: {}. Error: {}", path, e.what());
                return nullptr;
            }
        }

        //Ref<GLTexture> CreateTexture(const std::string& path)
        //{
        //    try
        //    {
        //        // Implement texture loading logic here
        //        Ref<GLTexture> texture = CreateRef<GLTexture>();
        //        // Load texture data...
        //        return texture;
        //    }
        //    catch (const std::exception& e)
        //    {
        //        ZERO_CORE_LOG_ERROR("Failed to load texture: {}. Error: {}", path, e.what());
        //        return nullptr;
        //    }
        //}

    private:
        static std::unordered_map<std::string, Ref<Model>> m_ModelResources;
        static std::unordered_map<std::string, Ref<GLTexture>> m_TextureResources;
        ModelImporter m_ModelImporter;

        // Helper type for the static_assert in CreateResource
        template<typename T>
        struct always_false : std::false_type {};
    };
}