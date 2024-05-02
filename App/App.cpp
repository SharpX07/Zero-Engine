#include "App.h"
#include <GL_graphics/Shader.h>
#include <Debug/Logger.h>
#include <vector>
#include <random>
#include <ext/matrix_transform.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <GL_graphics/Texture.h>
#include <Buffers/AttributeLayout.h>

namespace zeroengine
{
	void framebuffer_size_callback2(GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
	}
	zeroengine::GLTexture* text;

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
				text->SetTexture("C:/Users/Miguel/Documents/GitHub/Zero-engine/App/Assets/Textures/audrey.jpg");
				break;
			}
		}
	}
	App::App()
	{
		Logger logger;
		Resolution_ = glm::uvec2(800, 600);

		window.initialize();
		window.create(Resolution_.x, Resolution_.y, "Zero Engine");

		// Inicializar GLAD
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			logger.logError("Error al inicializar GLAD");
			return;
		}

		// Configurar la vista y el viewport
		glViewport(0, 0, 800, 600);
		glfwSetFramebufferSizeCallback(window.window, framebuffer_size_callback2);
		glfwSetKeyCallback(window.window, keyCallback);
		// Definir los v�rtices del tri�ngulo
		float vertices[] = {
			0.5f, 0.5f, 0.0f, 1.0f,1.0f, // top right
			0.5f, -0.5f, 0.0f, 1.0f,0.0f, // bottom right
			-0.5f, -0.5f, 0.0f, 0.0f,0.0f, // bottom left
			-0.5f, 0.5f, 0.0f, 0.0f,1.0f // top left
		};

		unsigned int indices[] = { // note that we start from 0!
			0, 1, 3, // first triangle
			1, 2, 3  // second triangle
		};
		// Crear un b�fer de v�rtices y enlazarlo
		VAO_ = new VAO();
		VAO_->Bind();
		VBO_ = new VBO(vertices, sizeof(vertices));
		VertexAttributeLayout layout;
		layout.addAttribute(GL_FLOAT, 3);
		layout.addAttribute(GL_FLOAT, 2);
		VAO_->AddBuffer(*VBO_, layout);
		EBO_ = new EBO(indices, sizeof(indices));

		// Desvincular VAO y VBO
		VAO_->UnBind();
		VBO_->UnBind();
		EBO_->UnBind();
	}

	void App::run()
	{
		Logger log;
		stbi_set_flip_vertically_on_load(true);

		const GLubyte* version = glGetString(GL_VERSION);
		log.logInfo("OpenGL version: " + std::string(reinterpret_cast<const char*>(version)));

		zeroengine::Shader shader("App/Assets/shaders/vertex.glsl", "App/Assets/shaders/fragment.glsl");
		// texture 1
		// ---------
		zeroengine::GLTexture texture2("App/Assets/Textures/sm.jpg");
		zeroengine::GLTexture texture("App/Assets/Textures/audrey.jpg");

		zeroengine::VertexAttributeLayout val;
		// Activar y vincular la primera textura
		texture.Bind(0);
		// Activar y vincular la segunda textura
		texture2.Bind(1);
		// Cargar la textura utilizando la clase Texture
		shader.Use();
		shader.setInt("ourTexture2", 1);
		shader.setInt("ourTexture", 0);
		while (!window.shouldClose()) {
			//Variables de uso general
			Time_ = glfwGetTime();
			// Procesar entrada
			processInput(window.window);
			// Limpiar el fondo
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			// Usar el programa de shaders
			shader.Use();
			shader.setFloat("uTime", (float)Time_);
			// Dibujar el triángulo
			VAO_->Bind();
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			window.update();
		}

	}

	void App::stop()
	{
		// Limpiar recursos
		delete VAO_;
		delete VBO_;
		delete EBO_;
		// Terminar GLFW
		glfwTerminate();
	}
}