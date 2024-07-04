#pragma once
#include <entt/entt.hpp>
#include <Scene/SceneCamera.h>
#include <Editor/EditorCamera.h>
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
		void SetEditorCamera(EditorCamera* camera) { m_EditorCamera = camera; }

		entt::entity GetPrincipalCamera() const { return m_PrincipalCamera; }
		EditorCamera* GetEditorCamera() const { return m_EditorCamera; }

	private:
		entt::registry m_Registry;
		entt::entity m_PrincipalCamera;
		EditorCamera* m_EditorCamera;
		friend class Entity;
	};
}