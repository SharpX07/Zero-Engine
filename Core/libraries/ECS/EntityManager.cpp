#include "EntityManager.h"
#include <string>
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
			m_Log.logError("No se pudo eliminar la identidad con ID: " + std::to_string(entity.Id));
	}

	void EntityManager::DeleteEntity(unsigned int id)
	{
		auto it = m_Entities.find(id);
		if (it != m_Entities.end())
			m_Entities.erase(it);
		else
			m_Log.logError("No se pudo eliminar la identidad con ID: " + std::to_string(id));
	}

	Entity* EntityManager::GetEntity(unsigned int id)
	{
		auto it = m_Entities.find(id);
		if (it != m_Entities.end())
			return &it->second;
		else
			m_Log.logError("No se pudo encontrar la identidad con ID: " + std::to_string(id));
	}

}
