#pragma once
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <Editor/Panel.h>

namespace Zero
{
	class LogPanel : public Panel
	{
	public:
		void OnRender() override;
	};
}
