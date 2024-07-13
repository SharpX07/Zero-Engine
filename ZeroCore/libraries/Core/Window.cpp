#include "Window.h"
#include <core/Logger.h>
#include <glad/glad.h>
#include <Events/KeyEvents.h>
#include <Events/MouseEvents.h>
#include <Events/WindowEvents.h>
#include <Core/Assert.h>

namespace Zero {

	Window::~Window() {
		glfwTerminate();
	}

	void Window::Initialize()
	{
		// Initialize GLFW
		if (!glfwInit()) {
			ZERO_CORE_LOG_CRITICAL("No se pudo inicializar GLFW");
			exit(EXIT_FAILURE);
		}
		ZERO_CORE_LOG_INFO("Window initialized successfully");
		// Set GLFW window hints
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	}

	void Window::Create(int width, int height, const char* title)
	{
		m_Data.Height = height;
		m_Data.Width = width;
		m_Data.Title = title;
		glfwWindowHandle = glfwCreateWindow(width, height, title, NULL, NULL);
		if (!glfwWindowHandle) {
			ZERO_CORE_LOG_ERROR("Error: Failed to create GLFW window");
			glfwTerminate();
			exit(EXIT_FAILURE);
		}
		ZERO_CORE_LOG_INFO("New window created: ( {0}, {1}, {2} )", title, width, height);
		glfwSetWindowUserPointer(glfwWindowHandle, &m_Data);
		glfwMakeContextCurrent(glfwWindowHandle);

		// KeyCallback
		glfwSetKeyCallback(glfwWindowHandle,
			[](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				switch (action)
				{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					data.EventCallbackFn(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					data.EventCallbackFn(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, true);
					data.EventCallbackFn(event);
					break;
				}
				}
			});
		// FrameBufferCallback

		glfwSetFramebufferSizeCallback(glfwWindowHandle,
			[](GLFWwindow* window, int width, int height)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				data.Width = width;
				data.Height = height;
				WindowResizeEvent event(width, height);
				data.EventCallbackFn(event);
				glViewport(0, 0, width, height);
			});
		//MouseButtonCallback
		glfwSetMouseButtonCallback(glfwWindowHandle,
			[](GLFWwindow* window, int button, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				switch (action)
				{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					data.EventCallbackFn(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data.EventCallbackFn(event);
					break;
				}
				}

			});
		//MouseScrollCallback
		glfwSetScrollCallback(glfwWindowHandle,
			[](GLFWwindow* window, double xoffset, double yoffset)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				MouseScrolledEvent event(xoffset, yoffset);
				data.EventCallbackFn(event);
			});
		glfwSwapInterval(1);
	}

	bool Window::ShouldClose() {
		return glfwWindowShouldClose(glfwWindowHandle);
	}
	bool Window::InitializeGLAD()
	{
		ZERO_ASSERT(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "No se pudo inicializar GLAD");
		ZERO_CORE_LOG_INFO("Glad initialized successfully");

		return true;
	}
	void Window::Update() {
		glfwSwapBuffers(glfwWindowHandle);
		glfwPollEvents();
	}

}