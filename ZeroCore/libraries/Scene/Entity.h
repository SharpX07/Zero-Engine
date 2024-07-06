#pragma once
#include <entt/entt.hpp>
#include "Scene.h"
#include <Core/Assert.h>
namespace Zero
{
	class Entity {
	public:
		Entity();
		Entity(entt::entity handler, Scene* scene);
		bool IsValid() { return m_EntityHandler != entt::null; }
		Scene* GetScene() { return m_Scene; }
		template<typename T, typename... Args>
		void AddComponent(Args&&... args)
		{
			ZERO_ASSERT(CRITICAL, !HasComponent<T>(), "La entidad ya tiene este componente");
			m_Scene->m_Registry.emplace<T>(m_EntityHandler, std::forward<Args>(args)...);
		}

		template<typename T, typename... Args>
		void AddOrReplaceComponent(Args&&... args)
		{
			m_Scene->m_Registry.emplace_or_replace<T>(m_EntityHandler, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent()
		{
			ZERO_ASSERT(CRITICAL,HasComponent<T>(),"La entidad no tiene este componente")
			return m_Scene->m_Registry.get<T>(m_EntityHandler);
		}

		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.all_of<T>(m_EntityHandler);
		}
		
		operator bool() const { return m_EntityHandler != entt::null; }

	private:
		entt::entity m_EntityHandler{ entt::null };
		Scene* m_Scene = nullptr;
		
		friend class Scene;
	};
}
