#pragma once
#include <Events/Event.h>
namespace Zero
{
	class Layer
	{
	public:
		virtual void OnEvent(Event& e);
		virtual void OnUpdate();

	private:
		static float m_ScrollDelta;
		static bool m_MouseScrolled;
	};
}