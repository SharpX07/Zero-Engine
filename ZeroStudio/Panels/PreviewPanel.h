#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <Scene/Entity.h>
#include <string>
#include <GLGraphics/FBO.h>
namespace Zero
{
	class PreviewPanel
	{
	public:
		PreviewPanel() = default;
		void OnRender();
		void SetCameraFocus(Entity& entity);
	private:
		Entity m_SelectedEntity;
		Framebuffer fbo{500,500};
	};
}
