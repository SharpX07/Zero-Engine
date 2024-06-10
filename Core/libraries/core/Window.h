#pragma once

#include <iostream>
#define GLFW_INCLUDE_NONE 
#include <GLFW/glfw3.h>
#include <Events/Event.h>
namespace Zero
{
	class Window {
	public:
		using EventCallbackFn = std::function<void(Event&)>;
		GLFWwindow* glfwWindowHandle;
		// Constructor
		Window() = default;

		// Destructor
		~Window();

		void Initialize();
		void Create(int width, int height, const char* title);
		bool ShouldClose();
		bool InitializeGLAD();
		void SetEventCallback(const EventCallbackFn& callback) { m_Data.EventCallbackFn = callback; }
		void Update();
		unsigned int GetWidth() const { return m_Data.Width; }
		unsigned int GetHeight() const { return m_Data.Height; }
		const std::string GetTitle() const { return m_Data.Title; }
		struct WindowData
		{	
			uint32_t Width;
			uint32_t Height;
			std::string Title;
			bool isVSync;
			WindowData(uint32_t width = 800, uint32_t height = 600, const std::string& title = "Zero Window", bool vsync = true)
				:Title(title), Width(width), Height(height), isVSync(vsync) {}
			EventCallbackFn EventCallbackFn;
		};
		WindowData m_Data;
	};
}