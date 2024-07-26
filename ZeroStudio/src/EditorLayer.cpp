#define GLM_ENABLE_EXPERIMENTAL
#include "EditorLayer.h"
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtx/string_cast.hpp>
#include <functional>
#include <core/Input.h>
#include <Events/KeyEvents.h>
#include <Events/MouseEvents.h>
#include <Events/WindowEvents.h>
#include <Scene/Scene.h>
#include <Scene/Entity.h>
#include <Scene/Components.h>
#include <Modules/Renderer.h>
#include <Modules/CameraUpdater.h>
#include <Modules/EntitySelector.h>
#include <nfd.h>
#include <core/Timer.h>
#include <ImGuizmo.h>

namespace Zero
{
	Editor::Editor()
	{
		newScene = nullptr;
		m_Logger.initialize();
		//this->Time_ = 0.0f;
		NFD_Init();
		m_Window.Initialize();
		m_Window.SetEventCallback(std::bind(&Editor::OnEvent, this, std::placeholders::_1));
		m_Resolution = { 1280,720 };
		m_Window.Create(m_Resolution.x, m_Resolution.y, "Zero Engine");
		m_Window.InitializeGLAD();
		Renderer::SetViewport(0, 0, 100, 800);
		Renderer::InitializeRenderer();
		Renderer::EnableCapability(GL_DEPTH_TEST);
		Renderer::EnableCapability(GL_BLEND);
		m_ImguiLayer.OnAttach();
		m_EditorViewPanel = CreateScope<EditorViewPanel>();
		m_HierarchyPanel = CreateScope<HierarchyPanel>();
		m_InspectorPanel = CreateScope<InspectorPanel>();
		m_PreviewPanel = CreateScope<PreviewPanel>();
	}

	GLFWwindow* Editor::GetGlfwWindow()
	{
		return m_Window.glfwWindowHandle;
	}

	void Editor::OnEvent(Event& e)
	{
		EventDispatcher Dispatcher(e);

		Dispatcher.Dispatch<WindowResizeEvent>([this](Event& e)
			{
				auto resizeEvent = static_cast<WindowResizeEvent&>(e);
				m_Resolution = {resizeEvent.GetWidth(), resizeEvent.GetHeight()};
				return true;
			});

		Input::OnEvent(e);
	}
	
	void Editor::Run()
	{
		newScene = CreateRef<Scene>();
		m_PreviewPanel->SetSceneFocus(newScene);
		m_HierarchyPanel->SetSceneFocus(newScene);
		Timer timer;
		timer.Start();
		while (!m_Window.ShouldClose()) {
			float deltaTime = timer.GetElapsedTime();
			timer.Reset();

			CameraSystem::UpdateCameras(*newScene);
			m_EditorViewPanel->UpdateEditorCamera(deltaTime);
			m_PreviewPanel->SetSceneFocus(newScene);
			m_EditorViewPanel->SetSceneFocus(newScene);
			m_InspectorPanel->SetEntityFocus(EntitySelector::GetEntitySelected());
			
			// Render Panelscls
			m_ImguiLayer.Begin();
			
			m_HierarchyPanel->OnRender();
			m_InspectorPanel->OnRender();
			m_PreviewPanel->OnRender();
			m_EditorViewPanel->OnRender();
			m_ImguiLayer.End();

			m_Window.Update();
		}
	}

	void Editor::Stop()
	{
		// Limpiar
		m_ImguiLayer.OnDetach();
		NFD_Quit();
		glfwDestroyWindow(m_Window.glfwWindowHandle);
		glfwTerminate();
	}
}