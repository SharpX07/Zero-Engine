#pragma once
#include <Core/Aliases.h>
#include <unordered_map>
#include <ResourceManagement/model.h>
#include <Modules/ModelReader.h>
#include <iostream>
#include <filesystem>
#include <Core/Logger.h>
#include <Core/Assert.h>
#include "ResourceManager.h"
namespace Zero
{
	std::unordered_map < std::string, Ref<Model >> ResourceManager::m_ModelResources;
	//Ref<Resource> ResourceManager::CreateResource(ResourceType type, const std::string& path)
	//{
	//	Ref<Resource> resource = CreateRef<Resource>();
	//	resource->Type = type;
	//	resource->Name = std::filesystem::path(path).filename().string();

	//	switch (type)
	//	{
	//	case MODEL_RESOURCE:
	//		resource->ModelResource = CreateModel(path);
	//		break;
	//	case TEXTURE_RESOURCE:
	//		//resource->TextureResource = CreateTexture(path);
	//		break;
	//	default:
	//		ZERO_ASSERT(ERROR, false, "Resource File not suported");
	//	}
	//	m_Resources[resource->Name] = resource;
	//	return resource;
	//}

	//Ref<Resource> ResourceManager::GetResource(const std::string& name)
	//{
	//	auto it = m_Resources.find(name);
	//	if (it != m_Resources.end()) {
	//		return it->second;
	//	}
	//	return nullptr;
	//}


}