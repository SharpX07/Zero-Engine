#include "Hierarchy.h"
#include <entt/entt.hpp>
#include <Core/Logger.h>
#include <Scene/Components.h>
#include <Core/UUID.h>
namespace Zero
{
	void HierarchyPanel::OnRender()
	{
		ImGui::Begin("Hierarchy");

		// Menú contextual para el panel de jerarquía
		if (ImGui::BeginPopupContextWindow("HierarchyContextMenu"))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
			{
				if (m_FocusedScene)
				{
					Entity newEntity = m_FocusedScene->CreateEntity();
					newEntity.AddComponent<IDComponent>(UUID());

					// Opcionalmente, puedes seleccionar la nueva entidad
					m_SelectedEntity = newEntity;
				}
			}

			// Puedes agregar más opciones aquí, por ejemplo:
			if (ImGui::MenuItem("Create Camera"))
			{
				if (m_FocusedScene)
				{
					Entity newCamera = m_FocusedScene->CreateEntity();
					
					Ref<SceneCamera> newCameraComponent = CreateRef<SceneCamera>(glm::mat4(1.0f), glm::mat4(1.0f));
					newCameraComponent.get()->SetPerspectiveProjection(glm::radians(45.0f), 0.1, 5000.0);
					newCameraComponent.get()->SetViewportSize(800, 800);
					newCameraComponent.get()->CalculateProjection();

					newCamera.AddComponent<CameraComponent>(newCameraComponent);

					m_SelectedEntity = newCamera;
				}
			}

			ImGui::EndPopup();
		}

		DrawEntityNodes();

		// Detectar clic derecho en el espacio vacío del panel
		if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsWindowHovered())
		{
			ImGui::OpenPopup("HierarchyContextMenu");
		}

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
			// Menú contextual para clic derecho
			if (ImGui::BeginPopupContextItem()) {
				if (ImGui::MenuItem("Delete Entity")) {
					// Lógica para borrar la entidad
					m_FocusedScene->DestroyEntity(m_FocusedScene->GetEntityByID(entity));
					ImGui::EndPopup();
					if (node_open) {
						ImGui::TreePop();
					}
					continue; // Saltar al siguiente ciclo ya que esta entidad ya no existe
				}
				// Puedes añadir más opciones al menú aquí
				ImGui::EndPopup();
			}
			if (node_open) {
				// Aquí puedes agregar más contenido para el nodo si es necesario
				ImGui::TreePop();
			}
		}
	}
}