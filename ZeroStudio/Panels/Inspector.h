#pragma once
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <Scene/Entity.h>
#include <string>

namespace Zero
{
	class InspectorPanel
	{
	public:
		void OnRender();
		void SetEntityFocus(Entity& entity);
	private:
		template<typename T, typename ImguiBlock>
		void DrawComponent(const std::string& name, ImguiBlock uiblock)
		{
			static bool firstShow = false;
			if(m_SelectedEntity.HasComponent<T>())
			{
				if (!firstShow) 
				{
					firstShow = true;
					ImGui::SetNextItemOpen(true);
				}

				auto& component = m_SelectedEntity.GetComponent<T>();
				if (ImGui::CollapsingHeader(name.c_str()))
				{
					uiblock(component);
				}
			}
		}

		void DrawComponents();
		Entity m_SelectedEntity;
	};
}
