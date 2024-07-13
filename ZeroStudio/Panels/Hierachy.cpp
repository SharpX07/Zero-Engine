#include "Hierarchy.h"
#include <entt/entt.hpp>
#include <Core/Logger.h>
#include <Scene/Components.h>
#include <Core/UUID.h>
#include <GLGraphics/ShaderParser.h>
#include <ResourceManagement/ResourceManager.h>
#include <nfd.h>
namespace Zero
{
	void HierarchyPanel::OnRender()
	{
		ImGui::Begin("Hierarchy");

		// Men� contextual para el panel de jerarqu�a
		if (ImGui::BeginPopupContextWindow("HierarchyContextMenu"))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
			{
				if (m_FocusedScene)
				{
					Entity newEntity = m_FocusedScene->CreateEntity();
					m_SelectedEntity = newEntity;
				}
			}

			// Puedes agregar m�s opciones aqu�, por ejemplo:
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
					newCamera.AddComponent<TransformComponent>();
					m_SelectedEntity = newCamera;
				}
			}
			if (ImGui::MenuItem("Create 3D Object"))
			{
				if (m_FocusedScene)
				{
					ShaderParser parser;
					ResourceManager manager;
					nfdu8char_t* outPath;
					nfdopendialogu8args_t args = { 0 };
					Entity new3DObject = m_FocusedScene->CreateEntity();
					new3DObject.AddComponent<TransformComponent>();
					new3DObject.AddComponent<ShaderComponent>(parser.GenerateShader("Assets/Shaders/Model.glsl"));
					nfdresult_t result = NFD_OpenDialogU8_With(&outPath, &args);
					if (result == NFD_OKAY)
					{
						new3DObject.AddComponent<MeshComponent>(manager.CreateResource<Model>(outPath));
						NFD_FreePathU8(outPath);
					}
					else if (result == NFD_CANCEL)
					{
						ZERO_APP_LOG_INFO("Model loader cancelled");
					}
					else
					{
						ZERO_APP_LOG_ERROR(NFD_GetError());
					}
					m_SelectedEntity = new3DObject;
				}
			}

			ImGui::EndPopup();
		}

		DrawEntityNodes();

		// Detectar clic derecho en el espacio vac�o del panel
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
			// Men� contextual para clic derecho
			if (ImGui::BeginPopupContextItem()) {
				if (ImGui::MenuItem("Delete Entity")) {
					// L�gica para borrar la entidad
					m_FocusedScene->DestroyEntity(m_FocusedScene->GetEntityByID(entity));
					ImGui::EndPopup();
					if (node_open) {
						ImGui::TreePop();
					}
					continue; // Saltar al siguiente ciclo ya que esta entidad ya no existe
				}
				// Puedes a�adir m�s opciones al men� aqu�
				ImGui::EndPopup();
			}
			if (node_open) {
				// Aqu� puedes agregar m�s contenido para el nodo si es necesario
				ImGui::TreePop();
			}
		}
	}
}