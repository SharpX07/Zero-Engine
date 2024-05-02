#include "App.h"
#include <GL_graphics/Shader.h>
#include <Debug/Logger.h>
#include <ext/matrix_transform.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <GL_graphics/Texture.h>
#include <Buffers/AttributeLayout.h>
#include <glm/glm.hpp>
#include <ext/matrix_clip_space.hpp>

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
		// Inicializar GLADZZ
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
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
			0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
			0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
			0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
			-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
			0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
			0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
			0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
			-0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
			-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
			-0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
			-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
			0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
			0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
			0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
			0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
			0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
			0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
			0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
			0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
			0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
			-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
			0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
			0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
			0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
			-0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
			-0.5f, 0.5f, -0.5f, 0.0f, 1.0f
		};

		// Crear un b�fer de v�rtices y enlazarlo
		VAO_ = new VAO();
		VAO_->Bind();
		VBO_ = new VBO(vertices, sizeof(vertices));
		VertexAttributeLayout layout;
		layout.addAttribute(GL_FLOAT, 3);
		layout.addAttribute(GL_FLOAT, 2);
		VAO_->AddBuffer(*VBO_, layout);

		// Desvincular VAO y VBO
		VAO_->UnBind();
		VBO_->UnBind();
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
		zeroengine::GLTexture texture("App/Assets/Textures/gato.jpg");

		zeroengine::VertexAttributeLayout val;
		// Activar y vincular la primera textura
		texture.Bind(0);
		// Cargar la textura utilizando la clase Texture
		shader.Use();
		shader.setInt("ourTexture", 0);
		glEnable(GL_DEPTH_TEST);

		float movex=0, movey = 0, movez = 0;

		while (!window.shouldClose()) {
			//Variables de uso general
			Time_ = glfwGetTime();
			// Procesar entrada
			processInput(window.window);
			// Limpiar el fondo
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			//codigo que usar movex,movey,movez para mover laca visión
			float speed = 0.1f; // Velocidad de movimiento de la cámara
			if (glfwGetKey(window.window, GLFW_KEY_W) == GLFW_PRESS) {
				movez -= speed; // Mueve la cámara hacia adelante en el eje z
			}
			if (glfwGetKey(window.window, GLFW_KEY_S) == GLFW_PRESS) {
				movez += speed; // Mueve la cámara hacia atrás en el eje z
			}
			if (glfwGetKey(window.window, GLFW_KEY_A) == GLFW_PRESS) {
				movex -= speed; // Mueve la cámara hacia la izquierda en el eje x
			}
			if (glfwGetKey(window.window, GLFW_KEY_D) == GLFW_PRESS) {
				movex += speed; // Mueve la cámara hacia la derecha en el eje x
			}
			if (glfwGetKey(window.window, GLFW_KEY_Q) == GLFW_PRESS) {
				movey -= speed; // Mueve la cámara hacia abajo en el eje y
			}
			if (glfwGetKey(window.window, GLFW_KEY_E) == GLFW_PRESS) {
				movey += speed; // Mueve la cámara hacia arriba en el eje y
			}
			// Usar el programa de shaders
			shader.Use();
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::rotate(model, glm::radians((float)Time_ * 200),
				glm::vec3(1.0f, 0.0f, 0.0f));

			glm::mat4 view = glm::mat4(1.0f);
			// note that we’re translating the scene in the reverse direction
			view = glm::translate(view, glm::vec3(movex,movey,movez));
			glm::mat4 projection(1.0f);
			projection = glm::perspective(glm::radians(45.0f), (float)Resolution_.x / (float)Resolution_.y, 0.1f, 100.0f);

			shader.setFloat("uTime", (float)Time_);
			shader.setMat4("model", model);
			shader.setMat4("projection", projection);
			shader.setMat4("view", view);
			// Dibujar el triángulo
			VAO_->Bind();
			glDrawArrays(GL_TRIANGLES, 0, 36);


			window.update();
			int x, y;
			glfwGetWindowSize(window.window, &x, &y);
			Resolution_ = glm::uvec2(x, y);
		}

	}

	void App::stop()
	{
		// Limpiar recursos
		delete VAO_;
		delete VBO_;
		// Terminar GLFW
		glfwTerminate();
	}
}