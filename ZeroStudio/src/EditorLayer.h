#pragma once
#include <Core/Application.h>
#include <Scene/Scene.h>
#include <Core/Logger.h>
#include <Core/Aliases.h>

namespace Zero
{
	class Editor : public Application
	{
	public:
		glm::uvec2 Resolution_;
		double Time_;
		Editor();
		GLFWwindow* GetGlfwWindow() override;
		void Run()  override;
		void Stop()  override;
		void OnEvent(Event& e);
	private:
		Zero::Window m_Window;
		Zero::Log m_Logger;
		Ref<Scene> newScene;
	};
}