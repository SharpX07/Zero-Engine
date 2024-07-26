#pragma once

#include <iostream>
#include <glm/glm.hpp>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <core/Window.h>

namespace Zero
{
	class Application
	{
	public:
		Application() { m_Instance = this; }
		static Application* GetInstance() { return m_Instance; }
		static glm::uvec2 GetResolution() { return m_Resolution; }
		virtual GLFWwindow* GetGlfwWindow() = 0;
		virtual void Run() = 0;
		virtual void Stop() = 0;
	protected:
		static Application* m_Instance;
		static glm::uvec2 m_Resolution;
	};
}
