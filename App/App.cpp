#include "App.h"
#include <GL_graphics/Shader.h>
#include <ext/matrix_transform.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <Buffers/AttributeLayout.h>
#include <glm/glm.hpp>
#include <ext/matrix_clip_space.hpp>
#include <ResourceManagement/Mesh.h>
#include <Modules/ModelReader.h>
#include <stb_image.h>

namespace Zero
{
	void framebuffer_size_callback2(GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
	}

	void processInput(GLFWwindow* window) {
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
	}

	void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		// Verifica si la tecla se presiona y no se suelta
		if (action == GLFW_PRESS || action == GLFW_REPEAT) {
			// Mueve el punto seg�n la tecla presionada
			switch (key) {
			case GLFW_KEY_W:
				break;
			}
		}
	}

	App::App()
	{
		Resolution_ = glm::uvec2(800, 600);
		window.Initialize();
		window.Create(Resolution_.x, Resolution_.y, "Zero Engine");
		if(!window.InitializeGLAD())
			m_Log.logError("Error al inicializar GLAD");
		m_RenderModule.SetViewport(0, 0, 800, 600);
		glfwSetFramebufferSizeCallback(window.glfwWindowHandle, framebuffer_size_callback2);
		glfwSetKeyCallback(window.glfwWindowHandle, keyCallback);
	}

	void App::run()
	{
		stbi_set_flip_vertically_on_load(true);
		// Render Configurations
		m_RenderModule.EnableCapability(GL_DEPTH_TEST);
		m_RenderModule.EnableCapability(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		Zero::ModelReader modelo("Assets/Models/airboat.obj");
		GLTexture texturaPrueba("Assets/Textures/sm.png");
		GLTexture texturaPrueba2("Assets/Textures/audrey.png");
		mesh::MeshTexture prueba = { &texturaPrueba , mesh::DIFFUSE ,"" };
		mesh::MeshTexture prueba2 = { &texturaPrueba2 , mesh::DIFFUSE ,"" };
		mesh_ = new Mesh(modelo.vertices, modelo.indices, {prueba,prueba2});

		Zero::Shader shader("Assets/shaders/vertex.glsl", "Assets/shaders/fragment.glsl");
		shader.Use();
		shader.setInt("ourTexture", 0);
		shader.setInt("ourTexture2", 1);

		float movex = 0, movey = 0, movez = 0;
		while (!window.ShouldClose()) {
			//Variables de uso general
			Time_ = glfwGetTime();
			// Procesar entrada
			processInput(window.glfwWindowHandle);
			// Limpiar el fondo
			m_RenderModule.Clear({ 84,84,128,255 });

			//codigo que usar movex,movey,movez para mover laca visión
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::rotate(model, glm::radians((float)Time_ * 100),
				glm::vec3(0.0f, 1.0f, 0.0f));

			glm::mat4 view = glm::mat4(1.0f);
			// note that we’re translating the scene in the reverse direction
			view = glm::translate(view, glm::vec3(0, 0, -10+-sin(Time_)*4));
			glm::mat4 projection(1.0f);
			projection = glm::perspective(glm::radians(45.0f), (float)Resolution_.x / (float)Resolution_.y, 0.1f, 100.0f);

			shader.setFloat("uTime", (float)Time_);
			shader.setMat4("model", model);
			shader.setMat4("projection", projection);
			shader.setMat4("view", view);
			// Dibujar el triángulo
			m_RenderModule.Render(*mesh_);

			window.Update();
			
			int x, y;
			glfwGetWindowSize(window.glfwWindowHandle, &x, &y);
			Resolution_ = glm::uvec2(x, y);
		}

	}

	void App::stop()
	{
		glfwTerminate();
	}
}