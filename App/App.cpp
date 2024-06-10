//#include <imgui.h>
//#include <imgui_impl_glfw.h>
//#include <imgui_impl_opengl3.h>

#include "app.h"
#include <GL_graphics/Shader.h>
#include <glm/ext/matrix_transform.hpp>
#include <Buffers/AttributeLayout.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <Modules/ModelReader.h>
#include <functional>
#include <Events/KeyEvents.h>
#include <core/Input.h>
namespace Zero
{
	Game::Game()
	{
		m_Logger.initialize();
		Resolution_ = glm::uvec2(800, 600);
		this->Time_ = 0.0f;
		m_Window.Initialize();
		m_Window.SetEventCallback(std::bind(&Game::OnEvent, this, std::placeholders::_1));
		m_Window.Create(Resolution_.x, Resolution_.y, "Zero Engine");
		if (!m_Window.InitializeGLAD())
			ZERO_CORE_LOG_CRITICAL("No se pudo inicializar GLAD");
		m_RenderModule.SetViewport(0, 0, 800, 600);
		m_RenderModule.InitializeRenderer();
	}

	GLFWwindow* Game::GetGlfwWindow()
	{
		return m_Window.glfwWindowHandle;
	}

	void Game::OnEvent(Event& e)
	{
		ZERO_APP_LOG_INFO("{0}", e.ToString());
	}

	void Game::Run()
	{
		// Render Configurations
		
		m_RenderModule.EnableCapability(GL_DEPTH_TEST);
		m_RenderModule.EnableCapability(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		Zero::ModelImporter ModelImporter;
		Model* modelo = ModelImporter.loadModel("Assets/Models/m2/scene.gltf");
		Zero::Shader shader("Assets/shaders/ModelVertex.glsl", "Assets/shaders/ModelFragment.glsl");
		Model* floor = ModelImporter.loadModel("Assets/Models/plano.gltf");
		Zero::Shader shaderFloor("Assets/shaders/vertex.glsl", "Assets/shaders/fragment.glsl");
		shader.Use();
		shader.setInt("ourTexture", 0);
		shader.setInt("ourTexture2", 1);
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(0, -15, -50));
		float ns = 0;
		float movex = 0, movey = 0, movez = 0;
		while (!m_Window.ShouldClose()) {
			//Variables de uso general
			Time_ = glfwGetTime();
			// Procesar entrada
			// Limpiar el fondo
				m_RenderModule.Clear({ 0,0,0,255 });
			

			//codigo que usar movex,movey,movez para mover laca visión
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::rotate(model, glm::radians((float)Time_ * 0),
				glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians((float)180),
				glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(100));
			// note that we’re translating the scene in the reverse direction
			ns += (float)cos(Time_) / 1000;
			view = glm::lookAt(glm::vec3(100 * sin(Time_), 100, 100 * cos(Time_)), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

			glm::mat4 projection(1.0f);
			projection = glm::perspective(glm::radians(45.0f), (float)Resolution_.x / (float)Resolution_.y, 0.1f, 5000.0f);
			shaderFloor.Use();
			shaderFloor.setFloat("uTime", (float)Time_);
			shaderFloor.setMat4("model", model);
			shaderFloor.setMat4("projection", projection);
			shaderFloor.setMat4("view", view);

			shader.Use();
			model = glm::mat4(1.0f);
			model = glm::rotate(model, glm::radians((float)Time_ * 0),
				glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.3, 0.3, 0.3));
			
			model = glm::translate(model, glm::vec3(0, 0, sin(Time_ * 0.432) * 20));

			if (Input::KeyPressed(KeyCode(Key::KEY_W)))
			{
			}
			shader.setFloat("uTime", (float)Time_);
			shader.setMat4("model", model);
			shader.setMat4("projection", projection);
			shader.setMat4("view", view);


			// Dibujar el triángulo
			shaderFloor.Use();
			m_RenderModule.Render(*floor, shaderFloor);
			shader.Use();
			m_RenderModule.Render(*modelo, shader);
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