#define GLM_ENABLE_EXPERIMENTAL
#include "EditorLayer.h"
#include <GLGraphics/Shader.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <Modules/ModelReader.h>
#include <functional>
#include <Events/KeyEvents.h>
#include <Events/MouseEvents.h>
#include <core/Input.h>
#include <Scene/Scene.h>
#include <Scene/Entity.h>
#include <Scene/Components.h>
#include <Modules/CameraUpdater.h>
#include <glm/gtx/string_cast.hpp>
#include <Editor/EditorCamera.h>
#include <Modules/Renderer.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLGraphics/FBO.h>
#include <Panels/Inspector.h>
#include <Panels/Hierarchy.h>
#include <Panels/PreviewPanel.h>
#include <Panels/EditorViewPanel.h>
#include <Core/UUID.h>
#include <ResourceManagement/ResourceManager.h>
namespace Zero
{
	Editor::Editor()
	{
		newScene = nullptr;
		m_Logger.initialize();
		Resolution_ = glm::uvec2(1280, 720);
		this->Time_ = 0.0f;
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
	}

	GLFWwindow* Editor::GetGlfwWindow()
	{
		return m_Window.glfwWindowHandle;
	}

	void Editor::OnEvent(Event& e)
	{
		Input::OnEvent(e);
		switch (e.GetEventType())
		{
		case EventType::WindowResized:
		{
			auto camera = newScene->GetPrincipalCamera();
			//newScene->GetRegistry().get<CameraComponent>(camera).camera.SetViewportSize(m_Window.GetWidth(), m_Window.GetHeight());
			auto editor = newScene->GetEditorCamera();
			//editor->SetViewportSize(m_Window.GetWidth(), m_Window.GetHeight());
			break;
		}
		}
	}

	void Editor::Run()
	{


		// Render Configurations
		Zero::ModelImporter ModelImporter;
		newScene = CreateRef<Scene>();
		Entity Modelo = newScene->CreateEntity();
		Modelo.AddComponent<TransformComponent>(glm::vec3(0, 0, 0), glm::vec3(-glm::radians(0.0f), 0, 0), glm::vec3(1.0f));
		//auto modelMs = ModelImporter.loadModel("Assets/Models/wall2.glb");
		ResourceManager manager;
		auto resource = manager.CreateResource<Model>("Assets/Models/wall2.glb");
		ZERO_APP_LOG_INFO(resource.use_count());
		Modelo.AddComponent<MeshComponent>(resource);
		resource.reset();
		Modelo.AddComponent<ShaderComponent>(CreateRef<Shader>("Assets/shaders/ModelVertex.glsl", "Assets/shaders/ModelFragment.glsl"));

		Ref<Entity> Camera = CreateRef<Entity>(newScene->CreateEntity());
		Ref<SceneCamera> newCamera = CreateRef<SceneCamera>(glm::mat4(1.0f), glm::mat4(1.0f));

		newCamera.get()->SetPerspectiveProjection(glm::radians(45.0f), 0.1, 5000.0);
		newCamera.get()->SetViewportSize(800, 800);
		newCamera.get()->CalculateProjection();

		Camera->AddComponent<CameraComponent>(newCamera);
		Camera->AddComponent<TransformComponent>(glm::vec3(0, 0, -4), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
		EditorCamera newEditorCamera = EditorCamera(glm::mat4(0.0f), glm::mat4(0.0f));
		newEditorCamera.SetPerspectiveProjection(glm::radians(45.0f), 0.1, 5000.0);
		newEditorCamera.SetViewportSize(800, 800);
		newEditorCamera.CalculateProjection();
		newEditorCamera.SetPosition({ 0,0,-4 });

		newScene->SetEditorCamera(&newEditorCamera);
		Framebuffer fbo = Framebuffer(500, 500);

		HierarchyPanel hierachy;
		PreviewPanel preview;
		InspectorPanel inspector;
		EditorViewPanel editorview;
		preview.SetSceneFocus(newScene);

		newScene->SetPrincipalCamera(Camera);
		
		while (!m_Window.ShouldClose()) {
			Time_ = glfwGetTime();
			newEditorCamera.Update(0.2f);
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

			hierachy.SetSceneFocus(newScene);
			hierachy.OnRender();

			inspector.SetEntityFocus(hierachy.GetEntityFocus());
			inspector.OnRender();

			preview.OnRender();

			editorview.SetSceneFocus(newScene);
			editorview.OnRender(newEditorCamera);


			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			m_Window.Update();
			int x, y;
			glfwGetWindowSize(m_Window.glfwWindowHandle, &x, &y);
			Resolution_ = glm::uvec2(x, y);
		}
	}

	void Editor::Stop()
	{
		// Limpiar
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
		glfwDestroyWindow(m_Window.glfwWindowHandle);
		glfwTerminate();
	}
}