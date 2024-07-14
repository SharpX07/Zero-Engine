#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <Scene/Scene.h>
#include <string>
#include <GLGraphics/FBO.h>
#include <Core/Aliases.h>
namespace Zero
{
	class PreviewPanel
	{
	public:
		PreviewPanel();
		void OnRender();
		void SetSceneFocus(Ref<Scene> scene);
	private:
		Ref<Scene> m_FocusedScene;
		Framebuffer m_FBO;
	};
}
