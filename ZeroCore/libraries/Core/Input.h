#pragma once

#include <Core/KeyCodes.h>
#include <Core/MouseCodes.h>
#include <glm/vec2.hpp>
#include <Events/Event.h>
namespace Zero
{
	class Input
	{
	public:
		static void Initialize();
		static void OnEvent(Event& e);

		static glm::vec2 GetMousePosition();
		static bool KeyPressed(KeyCode key);
		static bool KeyUp(KeyCode key);
		static bool KeyDown(KeyCode key);
		static bool MousePressed(MouseCode code);
		static bool MouseReleased(MouseCode code);
		static float GetMouseScrollDelta();
		static bool MouseScrolled();
		static void SetMouseScrollDelta(float delta) { m_ScrollDelta = delta; m_MouseScrolled = true; }

	private:
		static float m_ScrollDelta;
		static bool m_MouseScrolled;
	};
}