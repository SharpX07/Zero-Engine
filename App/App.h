#pragma once
#include <iostream>
#include <core/BaseApp.h>
#include <iostream>
#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <Buffers/VBO.h>
#include <Buffers/VAO.h>
#include <Buffers/EBO.h>
#include <core/Window.h>
#include <ResourceManagement/Mesh.h>
#include <glm/glm.hpp>
#include <Modules/Renderer.h>
#include <Debug/Logger.h>
namespace Zero
{
	void glfw_framebuffer_callback(GLFWwindow* window, int width, int height);

	class App
	{
	public:
		
		glm::uvec2 Resolution_;
		double Time_;
		//float TimeDelta_;
		//float FrameRate_;
		//int Frame_;

		Zero::Window window;
		Mesh *mesh_;
		App();
		void run();
		void stop();
	private:
		Zero::Renderer m_RenderModule;
		Zero::Logger  m_Log;
	};
}