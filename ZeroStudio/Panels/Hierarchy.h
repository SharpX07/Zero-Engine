#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <Scene/Scene.h>
#include <string>
#include <Core/Aliases.h>
#include <Scene/Entity.h>


namespace Zero
{
	class HierarchyPanel
	{
	public:
		void OnRender();
		void SetSceneFocus(Ref<Scene>  scene);
		Entity GetEntityFocus() { return m_SelectedEntity; };
	private:
		void DrawEntityNode(const std::string& name);
		void DrawEntityNodes();
		Ref<Scene> m_FocusedScene;
		Entity m_SelectedEntity{entt::null, m_FocusedScene.get()};
	};
}
