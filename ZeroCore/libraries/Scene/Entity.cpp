#pragma once
#include <entt/entt.hpp>
#include "Scene.h"
#include "Entity.h"
namespace Zero
{
	Entity::Entity() {}
	Entity::Entity(entt::entity handler, Scene* scene)
	{
		m_EntityHandler = handler;
		m_Scene = scene;
	}
}
