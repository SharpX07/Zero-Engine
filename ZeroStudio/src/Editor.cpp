#define GLM_ENABLE_EXPERIMENTAL
#include "Editor.h"
#include <glm/glm.hpp>
#include <functional>
#include <core/Input.h>
#include <core/Timer.h>
#include <Events/KeyEvents.h>
#include <Events/MouseEvents.h>
#include <Events/WindowEvents.h>
#include <Scene/Scene.h>
#include <Modules/CameraUpdater.h>
#include <Modules/Renderer.h>
#include <ResourceManagement/ResourceManager.h>
#include <nfd.h>
#include <Panels/ResourceExplorer.h>


namespace Zero
{
	ZeroStudio::ZeroStudio()
	{
		NFD_Init();
		m_Logger.Initialize();
		m_Window.Initialize();
		m_Resolution = { 1280,720 };
		m_Window.Create(m_Resolution.x, m_Resolution.y, "Zero Engine");
		m_Window.InitializeGLAD();
		m_Window.SetEventCallback(std::bind(&ZeroStudio::OnEvent, this, std::placeholders::_1));
		Renderer::InitializeRenderer();
		m_ImguiLayer.OnAttach();
		m_ImguiLayer.AddPanel("EditorViewPanel", CreateRef<EditorViewPanel>());
		m_ImguiLayer.AddPanel("HierarchyPanel", CreateRef<HierarchyPanel>());
		m_ImguiLayer.AddPanel("InspectorPanel", CreateRef<InspectorPanel>());
		m_ImguiLayer.AddPanel("PreviewPanel", CreateRef<PreviewPanel>());
		m_ImguiLayer.AddPanel("EditorViewPanel", CreateRef<EditorViewPanel>());
		m_ImguiLayer.AddPanel("ResourcesExplorer", CreateRef<ResourcesExplorer>());
		m_ImguiLayer.AddPanel("LogPanel", CreateRef<LogPanel>());
	}

	void ZeroStudio::OnEvent(Event& e)
	{
		EventDispatcher Dispatcher(e);

		Dispatcher.Dispatch<WindowResizeEvent>([this](Event& e)
			{
				auto resizeEvent = static_cast<WindowResizeEvent&>(e);
				m_Resolution = { resizeEvent.GetWidth(), resizeEvent.GetHeight() };
				return true;
			});
		Input::OnEvent(e);
		m_ImguiLayer.GetPanel<EditorViewPanel>("EditorViewPanel")->OnEvent(e);
	}

	void ZeroStudio::Run()
	{
		newScene = CreateRef<Scene>();
		m_ImguiLayer.GetPanel<EditorViewPanel>("EditorViewPanel")->SetSceneFocus(newScene);
		m_ImguiLayer.GetPanel<PreviewPanel>("PreviewPanel")->SetSceneFocus(newScene);
		m_ImguiLayer.GetPanel<HierarchyPanel>("HierarchyPanel")->SetSceneFocus(newScene);
		Timer timer;
		timer.Start();
		while (!m_Window.ShouldClose()) {
			float deltaTime = timer.GetElapsedTime();
			timer.Reset();
			// Update Systems
			CameraSystem::UpdateCameras(*newScene);
			m_ImguiLayer.GetPanel<EditorViewPanel>("EditorViewPanel")->UpdateEditorCamera(deltaTime);
			// Render Panels
			m_ImguiLayer.Begin();
			m_ImguiLayer.Render();
			m_ImguiLayer.End();
			// Update Window
			m_Window.Update();
		}
	}

	void ZeroStudio::Stop()
	{
		m_ImguiLayer.OnDetach();
		NFD_Quit();
		glfwDestroyWindow(m_Window.glfwWindowHandle);
		glfwTerminate();
	}
}