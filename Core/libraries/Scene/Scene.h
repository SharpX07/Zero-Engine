#pragma once
#include <entt/entt.hpp>
#include <Scene/SceneCamera.h>
namespace Zero
{
	class Entity;

	class Scene
	{
	public:
		Scene() = default;
		~Scene() = default;
		Entity CreateEntity();
		template<typename... Components>
		auto GetAllEntitiesWith() { return m_Registry.view<Components...>(); }
		entt::registry& GetRegistry() { return m_Registry; }
		void RenderScene();
		void SetPrincipalCamera(Entity& camera);
		entt::entity GetPrincipalCamera() const { return m_PrincipalCamera; }

	private:
		entt::registry m_Registry;
		entt::entity m_PrincipalCamera;
		friend class Entity;
	};
}