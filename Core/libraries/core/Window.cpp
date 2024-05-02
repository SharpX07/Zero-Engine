#include "Window.h"
#include <Debug/Logger.h>

namespace zeroengine {

	Window::~Window() {
		glfwTerminate();
	}

	void Window::initialize()
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

	void Window::create(int width, int height, const char* title)
	{
		glfwSwapInterval(1);
		// Create GLFW window
		window = glfwCreateWindow(width, height, title, NULL, NULL);
		if (!window) {
			std::cerr << "Error: Failed to create GLFW window" << std::endl;
			glfwTerminate();
			exit(EXIT_FAILURE);
		}
		glfwMakeContextCurrent(window);
	}

	bool Window::shouldClose() {
		return glfwWindowShouldClose(window);
	}

	void Window::update() {
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

}