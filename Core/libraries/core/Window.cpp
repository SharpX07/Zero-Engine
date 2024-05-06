#include "Window.h"
#include <Debug/Logger.h>
#include <glad/glad.h>

namespace Zero {

	Window::~Window() {
		glfwTerminate();
	}

	void Window::Initialize()
	{
		Logger log;
		// Initialize GLFW
		if (!glfwInit()) {
			log.logError("Error: Failed to initialize GLFW!");
			exit(EXIT_FAILURE);
		}

		// Set GLFW window hints
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	}

	void Window::Create(int width, int height, const char* title)
	{
		// Create GLFW window
		glfwWindowHandle = glfwCreateWindow(width, height, title, NULL, NULL);
		if (!glfwWindowHandle) {
			std::cerr << "Error: Failed to create GLFW window" << std::endl;
			glfwTerminate();
			exit(EXIT_FAILURE);
		}
		glfwMakeContextCurrent(glfwWindowHandle);
		//glfwSwapInterval(1);
	}

	bool Window::ShouldClose() {
		return glfwWindowShouldClose(glfwWindowHandle);
	}
	bool Window::InitializeGLAD()
	{
		return gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	}
	void Window::Update() {
		glfwSwapBuffers(glfwWindowHandle);
		glfwPollEvents();
	}

}