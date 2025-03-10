#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <Scene/Entity.h>
#include <GLGraphics/FBO.h>
#include <GLGraphics/VAO.h>
#include <GLGraphics/VBO.h>

#include <Core/Aliases.h>
#include <GLGraphics/Shader.h>



namespace Zero
{
	class EditorViewPanel
	{
	public:
		EditorViewPanel();
		void OnEvent(Event& e);
		void OnRender();
		void SetSceneFocus(Ref<Scene> scene);
		void UpdateEditorCamera(float deltaTime) { m_EditorCamera->Update(deltaTime); }
		Entity GetSelectedEntity() { return m_SelectedEntity; }
	private:
		void InitializeCamera();
		void InitializeGeometry();
		void InitializeShader();
		void DrawGrid();
		void DrawView();
		void HandleMousePick();
		void ManipulateObject(TransformComponent& transform);
		Ref<Scene> m_FocusedScene;
		Framebuffer m_FBO;
		Ref<Shader> m_TestShader;
		Scope<VAO> m_VAO;
		Scope<VBO> m_VBO;
		Scope<EditorCamera> m_EditorCamera;
		Entity m_SelectedEntity{ entt::null, m_FocusedScene.get() };
		bool m_IsFocused;
		bool m_PendingMousePickEvent;
	};
}
