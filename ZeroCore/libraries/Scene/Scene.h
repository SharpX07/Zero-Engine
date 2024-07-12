#pragma once
#include <entt/entt.hpp>
//#include <Scene/SceneCamera.h>
#include <Editor/EditorCamera.h>
#include <Core/Aliases.h>
namespace Zero
{
	class Entity;

	class Scene
	{
	public:
		Scene() = default;
		~Scene() = default;
		Entity CreateEntity();
		Entity CreateEntity(std::string name);
		void DestroyEntity(Entity entity);
		template<typename... Components>
		auto GetAllEntitiesWith() { return m_Registry.view<Components...>(); }
		auto GetAllEntities() { return m_Registry.view<entt::entity>(); }
		Entity GetEntityByID(entt::entity entity);
		entt::registry& GetRegistry() { return m_Registry; }
		void RenderScene();
		void SetPrincipalCamera(Ref<Entity> camera);
		//void SetEditorCamera(EditorCamera* camera) { m_EditorCamera = camera; }

		Ref<Entity> GetPrincipalCamera() const { return m_PrincipalCamera; }
		//EditorCamera* GetEditorCamera() const { return m_EditorCamera; }

	private:
		entt::registry m_Registry;
		Ref<Entity> m_PrincipalCamera;
		//EditorCamera* m_EditorCamera;
		friend class Entity;
	};
}