#pragma once
#define GLFW_INCLUDE_NONE
#include <iostream>
#include <glad/glad.h>
#include <GLGraphics/VBO.h>
#include <GLGraphics/VAO.h>
#include <GLGraphics/EBO.h>
#include <ResourceManagement/Mesh.h>
#include <glm/glm.hpp>
#include <Modules/Renderer.h>
#include <Core/Logger.h>
#include <Core/Application.h>
#include <Core/Window.h>

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
		Zero::Log m_Logger;
		Scene* newScene;
	};
}