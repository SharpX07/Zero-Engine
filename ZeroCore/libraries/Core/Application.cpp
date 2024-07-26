#include "Application.h"

namespace Zero
{
	Application* Application::m_Instance = nullptr;
	glm::uvec2 Application::m_Resolution = { 800,600 };
}