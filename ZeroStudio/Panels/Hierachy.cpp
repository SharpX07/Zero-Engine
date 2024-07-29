#include "Hierarchy.h"
#include <entt/entt.hpp>
#include <Core/Logger.h>
#include <Scene/Components.h>
#include <Core/UUID.h>
#include <GLGraphics/ShaderParser.h>
#include <ResourceManagement/ResourceManager.h>
#include <nfd.h>
#include <Modules/EntitySelector.h>
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
					new3DObject.AddComponent<ShaderComponent>(ResourceManager::GetInstance().CreateResource<Shader>("Assets/Shaders/EntityEditor.glsl"));
					nfdresult_t result = NFD_OpenDialogU8_With(&outPath, &args);
					if (result == NFD_OKAY)
					{
						new3DObject.AddComponent<MeshComponent>(manager.CreateResource<Model>(outPath));
						NFD_FreePathU8(outPath);
					}
					else if (result == NFD_CANCEL)
						ZERO_APP_LOG_INFO("Model loader cancelled");
					else
						ZERO_APP_LOG_ERROR(NFD_GetError());
					m_SelectedEntity = new3DObject;
				}
			}

			ImGui::EndPopup();
		}

		DrawEntityNodes();

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
			bool node_open;
			if (tempEntity.Tag.empty())
				node_open = ImGui::TreeNode("-");
			else
				node_open = ImGui::TreeNode(tempEntity.Tag.c_str());


			if (ImGui::IsItemClicked()) {
				EntitySelector::SetEntitySelected({ entity,m_FocusedScene.get() });
			}
			// Right click menu context
			if (ImGui::BeginPopupContextItem()) {
				if (ImGui::MenuItem("Delete Entity")) {
					m_FocusedScene->DestroyEntity(m_FocusedScene->GetEntityByID(entity));
					EntitySelector::SetEntitySelected({ entt::null,nullptr });
					ImGui::EndPopup();
					if (node_open) {
						ImGui::TreePop();
					}
					continue;
				}
				ImGui::EndPopup();
			}
			if (node_open) {
				ImGui::TreePop();
			}
		}
	}
}