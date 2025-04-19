#pragma once
#include <Core/Layer.h>
#include <Core/Aliases.h>
#include "Panel.h"
#include <string>
#include <memory>

namespace Zero
{
	class ImguiLayer : public Layer
	{
	public:
		ImguiLayer();
		~ImguiLayer();
		void OnEvent(Event& event) override;
		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate(float deltaTime) override;
		void OnImGuiRender();
		void Begin();
		void End();
		void AddPanel(std::string name, Ref<Panel> panel) { m_Panels.insert({ name, panel }); }
		void RemovePanel(std::string name);
		template<typename T>
		Ref<T> GetPanel(const std::string& name)
		{

			auto it = m_Panels.find(name);
			if (it == m_Panels.end())
				return nullptr; // Devolver nullptr si no se encuentra el panel

			Ref<T> value = std::dynamic_pointer_cast<T>(it->second);

			return value;
		}
		void Render();
	private:
		HashTable<std::string, Ref<Panel>> m_Panels;
	};
}