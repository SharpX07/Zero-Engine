#include <iostream>
#include "Scene.h"
#include "Entity.h"
#include <Scene/Components.h>
#include <Core/Aliases.h>
#include <Core/UUID.h>

namespace Zero
{
	Entity Scene::CreateEntity()
	{
		entt::entity entidad = m_Registry.create();
		Entity newEntity(entidad, this);
		std::string name = "Entity " + std::to_string((uint32_t)entidad);
		UUID id = UUID();
		newEntity.AddComponent<IDComponent>(id);
		newEntity.AddComponent<TagComponent>(name);
		m_EntityMap[id] = entidad;
		return Entity(entidad, this);
	}

	Entity Scene::CreateEntity(std::string name)
	{
		entt::entity entidad = m_Registry.create();
		Entity newEntity(entidad, this);
		newEntity.AddComponent<TagComponent>(name);
		return Entity(entidad, this);
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity.m_EntityHandler);
	}

	void Scene::SetPrincipalCamera(Ref<Entity> camera)
	{
		m_PrincipalCamera = camera;
	}

	/*Ref<Entity> Scene::GetPrincipalCamera() const
	{
		return m_PrincipalCamera;
	}*/

	Entity Scene::GetEntityByID(entt::entity entityID)
	{
		if (m_Registry.valid(entityID))
		{

			return Entity{ entityID, this };
		}
		// Podrías lanzar una excepción aquí o devolver una entidad nula
		return Entity{ entt::null, this };
	}

	Entity Scene::GetEntityByUUID(UUID id)
	{
		if (m_EntityMap.find(id) != m_EntityMap.end())
			return { m_EntityMap.at(id), this };

		return {};
	}
}