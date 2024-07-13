#pragma once
#include <entt/entt.hpp>
#include "Scene.h"
#include <Core/Assert.h>
#include "Components.h"
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
			if constexpr (std::is_same_v<T, IDComponent>)
			{
				// Caso especial para IDComponent
				ZERO_ASSERT(!HasComponent<T>(), "La entidad ya tiene este componente");
				m_Scene->m_Registry.emplace<T>(m_EntityHandler, std::forward<Args>(args)...);
				auto& idComponent = GetComponent<IDComponent>();
				ZERO_CORE_LOG_INFO("Add {0} component to the entity: {1}", typeid(T).name(), (uint64_t)idComponent.Id);
			}
			else
			{
				// Para todos los demás componentes
				auto& idComponent = GetComponent<IDComponent>();
				ZERO_ASSERT(!HasComponent<T>(), "La entidad ya tiene este componente");
				m_Scene->m_Registry.emplace<T>(m_EntityHandler, std::forward<Args>(args)...);
				ZERO_CORE_LOG_INFO("Add {0} component to the entity: {1}", typeid(T).name(), (uint64_t)idComponent.Id);
			}
		}

		template<typename T>
		void RemoveComponent()
		{
			ZERO_ASSERT(HasComponent<T>(), "La entidad no tiene este componente");
			m_Scene->m_Registry.remove<T>(m_EntityHandler);
		}

		template<typename T, typename... Args>
		void AddOrReplaceComponent(Args&&... args)
		{
			m_Scene->m_Registry.emplace_or_replace<T>(m_EntityHandler, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent()
		{
			ZERO_ASSERT(HasComponent<T>(), "La entidad no tiene este componente")
				return m_Scene->m_Registry.get<T>(m_EntityHandler);
		}

		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.all_of<T>(m_EntityHandler);
		}

		operator bool() const { return m_EntityHandler != entt::null; }
		operator entt::entity() const { return m_EntityHandler; }
		operator uint32_t() const { return (uint32_t)m_EntityHandler; }
	private:
		entt::entity m_EntityHandler{ entt::null };
		Scene* m_Scene = nullptr;

		friend class Scene;
	};
}
