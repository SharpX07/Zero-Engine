#pragma once
#include <Core/Application.h>
#include <Scene/Scene.h>
#include <Core/Logger.h>
#include <Core/Aliases.h>
#include <Panels/Inspector.h>
#include <Panels/Hierarchy.h>
#include <Panels/PreviewPanel.h>
#include <Panels/EditorViewPanel.h>
#include <Panels/LogPanel.h>
#include <Editor/ImguiLayer.h>
namespace Zero
{
	class ZeroStudio : public Application
	{
	public:

		ZeroStudio();
		GLFWwindow* GetGlfwWindow() override { return m_Window.glfwWindowHandle; };
		void Run()  override;
		void Stop()  override;
		void OnEvent(Event& e);
	private:
		Zero::Window m_Window;
		Zero::Log m_Logger;
		Ref<Scene> newScene = nullptr;
    // Panels
	private:
		ImguiLayer m_ImguiLayer;
		Scope<PreviewPanel> m_PreviewPanel;
		Scope<InspectorPanel> m_InspectorPanel;
		Scope<EditorViewPanel> m_EditorViewPanel;
		Scope<HierarchyPanel> m_HierarchyPanel;
		Scope<LogPanel> m_LogPanel;
	};
}