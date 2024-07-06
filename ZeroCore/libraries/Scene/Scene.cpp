#include <iostream>
#include "Scene.h"
#include "Entity.h"

namespace Zero
{
	Entity Scene::CreateEntity()
	{
		entt::entity entidad = m_Registry.create();

		return Entity(entidad, this);
	}
	void Scene::SetPrincipalCamera(Entity& camera)
	{
		m_PrincipalCamera = camera.m_EntityHandler;
	}

	Entity Scene::GetEntityByID(entt::entity entityID)
	{
		if (m_Registry.valid(entityID))
		{
			return Entity{ entityID, this };
		}
		// Podr�as lanzar una excepci�n aqu� o devolver una entidad nula
		return Entity{ entt::null, this };
	}
}