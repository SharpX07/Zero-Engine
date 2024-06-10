#pragma once
#define GLFW_INCLUDE_NONE
#include <iostream>
#include <glad/glad.h>
#include <Buffers/VBO.h>
#include <Buffers/VAO.h>
#include <Buffers/EBO.h>
#include <ResourceManagement/Mesh.h>
#include <glm/glm.hpp>
#include <Modules/Renderer.h>
#include <core/Logger.h>
#include <core/Application.h>
#include <core/Window.h>

namespace Zero
{
	class Game : public Application
	{
	public:
		glm::uvec2 Resolution_;
		double Time_;
		//float TimeDelta_;
		//float FrameRate_;
		//int Frame_;

		Game();
		GLFWwindow* GetGlfwWindow() override;
		void Run()  override;
		void Stop()  override;
		void OnEvent(Event& e);
	private:
		Zero::Window m_Window;
		Zero::Renderer m_RenderModule;
		Zero::Log m_Logger;
	};
}