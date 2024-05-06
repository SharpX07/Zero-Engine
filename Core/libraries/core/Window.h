#include <iostream>
#define GLFW_INCLUDE_NONE 
#include <GLFW/glfw3.h>

namespace Zero
{
	class Window {

	public:
		GLFWwindow* glfwWindowHandle;
		// Constructor
		Window() = default;

		// Destructor
		~Window();

		void Initialize();
		void Create(int width, int height, const char* title);
		// Function to check if window should close
		bool ShouldClose();
		bool InitializeGLAD();
		// Function to update the window
		void Update();
	};
}