#include "EntityManager.h"
#include <string>
#include <core/Logger.h>
namespace Zero
{
	EntityManager::~EntityManager()
	{
		m_Entities.clear();
	}

	Entity& EntityManager::CreateEntity()
	{
		Entity newEntity;
		m_Entities.insert({newEntity.Id,newEntity});
		return newEntity;
	}

	void EntityManager::DeleteEntity(Entity entity)
	{
		auto it = m_Entities.find(entity.Id);
		if (it != m_Entities.end())
			m_Entities.erase(it);
		else
			ZERO_CORE_LOG_ERROR("No se pudo eliminar la identidad con ID: {0}", entity.Id);
	}

	void EntityManager::DeleteEntity(unsigned int id)
	{
		auto it = m_Entities.find(id);
		if (it != m_Entities.end())
			m_Entities.erase(it);
		else
			ZERO_CORE_LOG_ERROR("No se pudo eliminar la identidad con ID: {0}", id);
	}

	Entity* EntityManager::GetEntity(unsigned int id)
	{
		auto it = m_Entities.find(id);
		if (it != m_Entities.end())
			return &it->second;
		else
			ZERO_CORE_LOG_ERROR("No se pudo eliminar la identidad con ID: {0}", id);
	}
}
