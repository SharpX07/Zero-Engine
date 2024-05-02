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

namespace zeroengine
{
	void glfw_framebuffer_callback(GLFWwindow* window, int width, int height);

	class App
	{
	public:
		glm::uvec2 Resolution_;
		double Time_;
		float TimeDelta_;
		float FrameRate_;
		int Frame_;

		zeroengine::Window window;
		unsigned int shaderProgram;
		VBO* VBO_;
		VAO* VAO_;
		EBO* EBO_;
		App();
		void run();
		void stop();
	private:

	};
}