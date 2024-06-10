#pragma once

#include <Core/KeyCodes.h>
#include <Core/MouseCodes.h>
#include <glm/vec2.hpp>
namespace Zero
{
	class Input
	{
	public:
		static glm::vec2 GetMousePosition();
		static bool KeyPressed(KeyCode key);
		static bool MousePressed(MouseCode code);
	};
}