#include "Input.h"
#include <Core/Application.h>
#include <map>
#include <Core/Logger.h>
#include <Events/MouseEvents.h>
namespace Zero
{
	float Input::m_ScrollDelta = 0;
	bool Input::m_MouseScrolled = false;

	void Input::Initialize()
	{

	}

	void Input::OnEvent(Event& e)
	{
		switch (e.GetEventType())
		{
		case EventType::MouseScrolled:
		{
			auto& scrolledEvent = static_cast<MouseScrolledEvent&>(e);
			Input::SetMouseScrollDelta(scrolledEvent.GetYOffset());
			break;
		}
		}
	}
	glm::vec2 Input::GetMousePosition()
	{
		auto window = Application::GetInstance()->GetGlfwWindow();
		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);
		return glm::vec2(static_cast<float>(xPos), static_cast<float>(yPos));
	}
	bool Input::KeyPressed(Key key)
	{
		auto window = Application::GetInstance()->GetGlfwWindow();
		auto state = glfwGetKey(window, static_cast<int32_t>(key));
		return state == GLFW_PRESS;
	}
	bool Input::KeyUp(Key key) {
		auto window = Application::GetInstance()->GetGlfwWindow();
		static std::map<Key, bool> keyStates; // Store key states
		bool wasPressed = keyStates[key];
		auto state = glfwGetKey(window, static_cast<int32_t>(key));
		keyStates[key] = state == GLFW_PRESS;
		return wasPressed && state == GLFW_RELEASE;
	}

	bool Input::KeyDown(Key key) {
		auto window = Application::GetInstance()->GetGlfwWindow();
		static std::map<Key, bool> keyStates; // Store key states
		bool wasPressed = keyStates[key];
		auto state = glfwGetKey(window, static_cast<int32_t>(key));
		keyStates[key] = state == GLFW_RELEASE;
		return wasPressed && state == GLFW_PRESS;
	}
	bool Input::MousePressed(MouseCode code)
	{
		auto window = Application::GetInstance()->GetGlfwWindow();
		auto state = glfwGetMouseButton(window, static_cast<int32_t>(code));
		return state == GLFW_PRESS;
	}

	bool Input::MouseReleased(MouseCode code)
	{
		auto window = Application::GetInstance()->GetGlfwWindow();
		static auto lastState = glfwGetMouseButton(window, static_cast<int32_t>(code));
		glfwPollEvents();  // Asegurarse de procesar eventos para actualizar el estado

		auto currentState = glfwGetMouseButton(window, static_cast<int32_t>(code));
		return lastState == GLFW_PRESS && currentState == GLFW_RELEASE;
	}

	float Input::GetMouseScrollDelta()
	{
		auto window = Application::GetInstance()->GetGlfwWindow();
		auto value = m_ScrollDelta;
		m_ScrollDelta = 0;
		return value;
	}
	bool Input::MouseScrolled()
	{
		bool scrl = m_MouseScrolled;
		m_MouseScrolled = false;
		return scrl;
	}
}

