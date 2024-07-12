#define GLM_ENABLE_EXPERIMENTAL
#include "EditorLayer.h"
#include <GLGraphics/Shader.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <functional>
#include <Events/KeyEvents.h>
#include <Events/MouseEvents.h>
#include <Events/WindowEvents.h>
#include <core/Input.h>
#include <Scene/Scene.h>
#include <Scene/Entity.h>
#include <Scene/Components.h>
#include <Modules/CameraUpdater.h>
#include <glm/gtx/string_cast.hpp>
#include <Modules/Renderer.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <ResourceManagement/ResourceManager.h>
#include <nfd.h>
namespace Zero
{
	Editor::Editor()
	{
		newScene = nullptr;
		m_Logger.initialize();
		Resolution_ = glm::uvec2(1280, 720);
		this->Time_ = 0.0f;
		NFD_Init();
		m_Window.Initialize();
		m_Window.SetEventCallback(std::bind(&Editor::OnEvent, this, std::placeholders::_1));
		m_Window.Create(Resolution_.x, Resolution_.y, "Zero Engine");
		m_Window.InitializeGLAD();
		Renderer::SetViewport(0, 0, 100, 800);
		Renderer::InitializeRenderer();
		Renderer::EnableCapability(GL_DEPTH_TEST);
		Renderer::EnableCapability(GL_BLEND);
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(m_Window.glfwWindowHandle, true);
		ImGui_ImplOpenGL3_Init("#version 330");

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
				Resolution_ = glm::uvec2(resizeEvent.GetWidth(), resizeEvent.GetHeight());
				return true;
			});

		Input::OnEvent(e);
	}

	void Editor::Run()
	{
		ResourceManager manager;
		newScene = CreateRef<Scene>();
		Entity Modelo = newScene->CreateEntity();
		Modelo.AddComponent<TransformComponent>(glm::vec3(0, 0, 0), glm::vec3(-glm::radians(0.0f), 0, 0), glm::vec3(1.0f));
		Modelo.AddComponent<MeshComponent>(manager.CreateResource<Model>("Assets/Models/wal2l2.glb"));
		Modelo.AddComponent<ShaderComponent>(CreateRef<Shader>("Assets/shaders/ModelVertex.glsl", "Assets/shaders/ModelFragment.glsl"));
		m_PreviewPanel->SetSceneFocus(newScene);
		m_HierarchyPanel->SetSceneFocus(newScene);
		while (!m_Window.ShouldClose()) {
			Time_ = glfwGetTime();
			m_EditorViewPanel->UpdateEditorCamera(0.2f);
			m_InspectorPanel->SetEntityFocus(m_HierarchyPanel->GetEntityFocus());
			m_EditorViewPanel->SetSceneFocus(newScene);
			CameraSystem::UpdateCameras(*newScene);
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			Renderer::Clear({ 0,0,0,255 });
			// Iniciar nuevo frame de ImGui
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			// Crear ventana de dockspace
			ImGui::DockSpaceOverViewport();
			// Render Panels
			m_HierarchyPanel->OnRender();
			m_InspectorPanel->OnRender();
			m_PreviewPanel->OnRender();
			m_EditorViewPanel->OnRender();
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			m_Window.Update();
		}
	}

	void Editor::Stop()
	{
		// Limpiar
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
		NFD_Quit();
		glfwDestroyWindow(m_Window.glfwWindowHandle);
		glfwTerminate();
	}
}