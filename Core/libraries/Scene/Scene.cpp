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
}