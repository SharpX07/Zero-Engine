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
	HashTable<std::string, Ref<Resource>> ResourceManager::m_Resources = {};
}