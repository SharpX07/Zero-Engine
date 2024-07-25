#pragma once
#include <Events/Event.h>
namespace Zero
{
	class Layer
	{
	public:
		Layer();
		virtual ~Layer();
		virtual void OnEvent(Event &e) {}
		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(float deltaTime) {}
		bool IsActive() { return m_IsActive; }
		bool SetActive(bool active) { m_IsActive = active; }

	protected:
		bool m_IsActive;
	};
}