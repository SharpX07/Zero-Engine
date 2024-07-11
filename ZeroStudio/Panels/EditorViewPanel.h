#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <Scene/Entity.h>
#include <GLGraphics/FBO.h>
#include <Core/Aliases.h>
#include <GLGraphics/Shader.h>
namespace Zero
{
	class EditorViewPanel
	{
	public:
		EditorViewPanel()
		{
			m_TestShader = CreateScope<Shader>("Assets/Shaders/Grid.vert", "Assets/Shaders/Grid.frag");
		}
		void OnRender(EditorCamera& editorCamera);
		void SetSceneFocus(Ref<Scene> scene);
	private:
		Ref<Scene> m_FocusedScene;
		Framebuffer m_FBO{ 512,512 };
		Scope<Shader> m_TestShader;
	};
}
