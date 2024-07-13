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
			if (m_SelectedEntity.HasComponent<T>())
			{
				auto& component = m_SelectedEntity.GetComponent<T>();

				ImGui::PushID(name.c_str());

				// Comenzamos un grupo para poder posicionar el botón absolutamente
				ImGui::BeginGroup();

				// Calculamos el ancho del header menos el espacio para el botón
				float headerWidth = ImGui::GetContentRegionAvail().x - 30;

				// Dibujamos el header con un ancho específico
				bool open = ImGui::CollapsingHeader(name.c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap);

				// Posicionamos el botón a la derecha del header
				ImGui::SameLine(headerWidth);
				if (ImGui::Button("..."))
				{
					ImGui::OpenPopup("ComponentContextMenu");
				}

				// Terminamos el grupo
				ImGui::EndGroup();

				if (ImGui::BeginPopup("ComponentContextMenu"))
				{
					if (ImGui::MenuItem("Remove Component"))
					{
						m_SelectedEntity.RemoveComponent<T>();
						ImGui::EndPopup();
						ImGui::PopID();
						return;
					}
					if (ImGui::MenuItem("Set to Default"))
					{
						//component.Reset();
						ImGui::EndPopup();
						ImGui::PopID();
						return;
					}
					ImGui::EndPopup();
				}

				if (open)
				{
					ImGui::Indent(20.0f);
					uiblock(component);
					ImGui::Unindent(20.0f);
				}

				ImGui::PopID();
			}
		}

		void DrawComponents();
		Entity m_SelectedEntity;
	};
}
