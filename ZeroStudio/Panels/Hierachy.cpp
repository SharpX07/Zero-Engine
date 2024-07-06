#include "Hierarchy.h"
#include <entt/entt.hpp>
#include <Core/Logger.h>
#include <Scene/Components.h>
namespace Zero
{
	void HierarchyPanel::OnRender()
	{
		ImGui::Begin("Hierachy");
		DrawEntityNodes();
		ImGui::End();
	}
	void HierarchyPanel::SetSceneFocus(Ref<Scene> scene)
	{
		m_FocusedScene = scene;
	}
	void HierarchyPanel::DrawEntityNode(const std::string& name)
	{
	}
	void HierarchyPanel::DrawEntityNodes()
	{
		auto view = m_FocusedScene->GetAllEntities();
		for (auto entity : view) {
			 
			auto tempEntity = m_FocusedScene->GetEntityByID(entity).GetComponent<TagComponent>();
			bool node_open = ImGui::TreeNode(tempEntity.Tag.c_str());

			if (ImGui::IsItemClicked()) {
				// El nodo fue clickeado
				m_SelectedEntity = m_FocusedScene->GetEntityByID(entity);
			}

			if (node_open) {
				// Aquí puedes agregar más contenido para el nodo si es necesario
				ImGui::TreePop();
			}
		}
	}
}