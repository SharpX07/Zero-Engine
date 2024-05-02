#include <iostream>
#include <GLFW/glfw3.h>

namespace zeroengine
{
	class Window {

	public:
		GLFWwindow* window;
		// Constructor
		Window() = default;

		// Destructor
		~Window();

		void initialize();
		void create(int width, int height, const char* title);
		// Function to check if window should close
		bool shouldClose();

		// Function to update the window
		void update();
	};
}