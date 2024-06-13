//#include <imgui.h>
//#include <imgui_impl_glfw.h>
//#include <imgui_impl_opengl3.h>

#include "app.h"
#include <GLGraphics/Shader.h>
#include <glm/ext/matrix_transform.hpp>
#include <GLGraphics/AttributeLayout.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <Modules/ModelReader.h>
#include <functional>
#include <Events/KeyEvents.h>
#include <core/Input.h>
#include <Scene/Scene.h>
#include <Scene/Entity.h>
#include <Scene/Components.h>
#include <Modules/CameraUpdater.h>
namespace Zero
{
	Game::Game()
	{
		newScene = nullptr;
		m_Logger.initialize();
		Resolution_ = glm::uvec2(800, 600);
		this->Time_ = 0.0f;
		m_Window.Initialize();
		m_Window.SetEventCallback(std::bind(&Game::OnEvent, this, std::placeholders::_1));
		m_Window.Create(Resolution_.x, Resolution_.y, "Zero Engine");
		if (!m_Window.InitializeGLAD())
			ZERO_CORE_LOG_CRITICAL("No se pudo inicializar GLAD")
			Renderer::SetViewport(0, 0, 800, 600);
		Renderer::InitializeRenderer();
		Renderer::EnableCapability(GL_DEPTH_TEST);
		Renderer::EnableCapability(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	GLFWwindow* Game::GetGlfwWindow()
	{
		return m_Window.glfwWindowHandle;
	}

	void Game::OnEvent(Event& e)
	{
		switch (e.GetEventType())
		{
		case EventType::WindowResized:
			auto camera = newScene->GetPrincipalCamera();
			newScene->GetRegistry().get<CameraComponent>(camera).camera.SetViewportSize(m_Window.GetWidth(), m_Window.GetHeight());
			break;
		}
	}

	void Game::Run()
	{
		// Render Configurations
		Zero::ModelImporter ModelImporter;
		newScene = new Scene();
		Entity Modelo = newScene->CreateEntity();
		Modelo.AddComponent<TransformComponent>(glm::vec3(0, 0, 0), glm::vec3(3.14, 0, 0), glm::vec3(0.3, 0.3, 0.3));

		auto modelMs = ModelImporter.loadModel("Assets/Models/m2/scene.gltf");
		Modelo.AddComponent<MeshComponent>(*modelMs);
		Shader shader("Assets/shaders/ModelVertex.glsl", "Assets/shaders/ModelFragment.glsl");
		Modelo.AddComponent<ShaderComponent>(shader);

		Entity Floor = newScene->CreateEntity();
		Floor.AddComponent<TransformComponent>(glm::vec3(0, 0, 0), glm::vec3(3.14, 0, 0), glm::vec3(100));
		auto floorMs = ModelImporter.loadModel("Assets/Models/plano.gltf");
		Floor.AddComponent<MeshComponent>(*floorMs);
		Shader shaderFloor("Assets/shaders/vertex.glsl", "Assets/shaders/fragment.glsl");
		Floor.AddComponent<ShaderComponent>(shaderFloor);

		Entity Camera = newScene->CreateEntity();
		SceneCamera newCamera = SceneCamera(glm::mat4(0.0f), glm::mat4(0.0f));
		newCamera.SetPerspectiveProjection((float)glm::radians(45.0f), 0.1, 5000.0);
		newCamera.SetViewportSize(800, 600);
		newCamera.CalculateProjection();

		Camera.AddComponent<CameraComponent>(newCamera);
		Camera.AddComponent<TransformComponent>(glm::vec3(0, 0, -30), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
		while (!m_Window.ShouldClose()) {
			Time_ = glfwGetTime();
			Renderer::Clear({ 0,0,0,255 });
			CameraSystem::UpdateCameras(*newScene);

			TransformComponent tempCameraTransform = Camera.GetComponent<TransformComponent>();

			tempCameraTransform.Translation = glm::vec3(0, sin(Time_) * 10, -20);
			Camera.AddOrReplaceComponent<TransformComponent>(tempCameraTransform.Translation, tempCameraTransform.Rotation, tempCameraTransform.Scale);
			newScene->SetPrincipalCamera(Camera);

			TransformComponent tc = Modelo.GetComponent<TransformComponent>();
			Modelo.AddOrReplaceComponent<TransformComponent>(tc.Translation, tc.Rotation + glm::vec3(0.1, 0, 0), tc.Scale);

			Renderer::Render(*newScene);
			m_Window.Update();

			int x, y;
			glfwGetWindowSize(m_Window.glfwWindowHandle, &x, &y);
			Resolution_ = glm::uvec2(x, y);
		}
	}

	void Game::Stop()
	{
		glfwTerminate();
	}
}