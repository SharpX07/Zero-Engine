//#include <imgui.h>
//#include <imgui_impl_glfw.h>
//#include <imgui_impl_opengl3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include "app.h"
#include <GLGraphics/Shader.h>
#include <glm/ext/matrix_transform.hpp>
#include <GLGraphics/AttributeLayout.h>
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
namespace Zero
{
	Game::Game()
	{
		newScene = nullptr;
		m_Logger.initialize();
		Resolution_ = glm::uvec2(800, 800);
		this->Time_ = 0.0f;
		m_Window.Initialize();
		m_Window.SetEventCallback(std::bind(&Game::OnEvent, this, std::placeholders::_1));
		m_Window.Create(Resolution_.x, Resolution_.y, "Zero Engine");

		ZERO_ASSERT(CRITICAL, m_Window.InitializeGLAD(), "No se pudo inicializar GLAD");
		ZERO_CORE_LOG_INFO("Glad initialized successfully");
		Renderer::SetViewport(0, 0, 800, 800);
		Renderer::InitializeRenderer();
		Renderer::EnableCapability(GL_DEPTH_TEST);
		Renderer::EnableCapability(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
	}

	GLFWwindow* Game::GetGlfwWindow()
	{
		return m_Window.glfwWindowHandle;
	}

	void Game::OnEvent(Event& e)
	{

		switch (e.GetEventType())
		{
		case EventType::MouseScrolled:
		{
			auto& scrolledEvent = dynamic_cast<MouseScrolledEvent&>(e);
			Input::SetMouseScrollDelta(scrolledEvent.GetYOffset());
			break;
		}
		case EventType::WindowResized:
		{
			auto camera = newScene->GetPrincipalCamera();
			newScene->GetRegistry().get<CameraComponent>(camera).camera.SetViewportSize(m_Window.GetWidth(), m_Window.GetHeight());
			auto editor = newScene->GetEditorCamera();
			editor->SetViewportSize(m_Window.GetWidth(), m_Window.GetHeight());
			break;
		}
		}
	}

	void Game::Run()
	{


		// Render Configurations
		Zero::ModelImporter ModelImporter;
		newScene = new Scene();
		Entity Modelo = newScene->CreateEntity();
		Modelo.AddComponent<TransformComponent>(glm::vec3(0, 0, 0), glm::vec3(3.14/2, 0, 0), glm::vec3(0.01f));

		auto modelMs = ModelImporter.loadModel("Assets/Models/m2/scene.gltf");
		Modelo.AddComponent<MeshComponent>(modelMs);
		Shader shader("Assets/shaders/ModelVertex.glsl", "Assets/shaders/ModelFragment.glsl");
		Modelo.AddComponent<ShaderComponent>(shader);

		Entity Camera = newScene->CreateEntity();
		SceneCamera newCamera = SceneCamera(glm::mat4(0.0f), glm::mat4(0.0f));
		newCamera.SetPerspectiveProjection(glm::radians(45.0f), 0.1, 5000.0);
		newCamera.SetViewportSize(800, 800);
		newCamera.CalculateProjection();

		Camera.AddComponent<CameraComponent>(newCamera);
		Camera.AddComponent<TransformComponent>(glm::vec3(0, 0, -4), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));

		EditorCamera newEditorCamera = EditorCamera(glm::mat4(0.0f), glm::mat4(0.0f));
		newEditorCamera.SetPerspectiveProjection(glm::radians(45.0f), 0.1, 5000.0);
		newEditorCamera.SetViewportSize(800, 800);
		newEditorCamera.CalculateProjection();
		newEditorCamera.SetPosition({ 0,0,-4 });

		newScene->SetEditorCamera(&newEditorCamera);
		Shader testShader("Assets/Shaders/Grid.vert", "Assets/Shaders/Grid.frag");

		while (!m_Window.ShouldClose()) {
			Time_ = glfwGetTime();
			Renderer::Clear({ 0,0,0,255 });
			newEditorCamera.Update(0.2f);

			CameraSystem::UpdateCameras(*newScene);

			TransformComponent& tempCameraTransform = Camera.GetComponent<TransformComponent>();
			tempCameraTransform.Translation = glm::vec3(0, sin(Time_ / 1) * 4, cos(Time_ / 1) * 4);
			newScene->SetPrincipalCamera(Camera);

			TransformComponent& tc = Modelo.GetComponent<TransformComponent>();
			//tc.Rotation += glm::vec3(0, 0.01, 0);
			//Renderer::RenderOnRuntime(*newScene);
			Renderer::RenderOnEditor(*newScene, newEditorCamera);
			testShader.Use();
			//testShader.setMat4("view", Camera.GetComponent<CameraComponent>().camera.GetView());
			//testShader.setMat4("projection", Camera.GetComponent<CameraComponent>().camera.GetProjection());
			//testShader.setVec3("pos", Camera.GetComponent<TransformComponent>().Translation);

			testShader.setMat4("view", newEditorCamera.GetView());
			testShader.setMat4("projection", newEditorCamera.GetProjection());
			testShader.setVec3("pos", newEditorCamera.GetPosition());
			glDrawArrays(GL_TRIANGLES, 0, 6);

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