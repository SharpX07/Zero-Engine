#include "Inspector.h"
#include <Scene/Components.h>
#include <imgui_stdlib.h>
#include <imgui_internal.h>
#include <ResourceManagement/ResourceManager.h>
#include <nfd.h>

namespace Zero
{
	void InspectorPanel::OnRender()
	{
		ImGui::Begin("Inspector");
		if (m_SelectedEntity.IsValid())
		{
			if (ImGui::Button("Add Component"))
				ImGui::OpenPopup("AddComponentPopup");

			if (ImGui::BeginPopup("AddComponentPopup"))
			{
				if (ImGui::MenuItem("Transform Component"))
				{
					if (!m_SelectedEntity.HasComponent<TransformComponent>())
						m_SelectedEntity.AddComponent<TransformComponent>();
					else
						ZERO_APP_LOG_INFO("This entity already has a Transform Component!");
				}
				if (ImGui::MenuItem("Camera Component"))
				{
					if (!m_SelectedEntity.HasComponent<CameraComponent>())
					{
						Ref<SceneCamera> newCamera = CreateRef<SceneCamera>(glm::mat4(1.0f), glm::mat4(1.0f));
						newCamera.get()->SetPerspectiveProjection(glm::radians(45.0f), 0.1, 5000.0);
						newCamera.get()->SetViewportSize(800, 800);
						newCamera.get()->CalculateProjection();

						m_SelectedEntity.AddComponent<CameraComponent>(newCamera);
					}
					else
						ZERO_APP_LOG_INFO("This entity already has a Camera Component!");
				}
				if (ImGui::MenuItem("Mesh Component"))
				{
					if (!m_SelectedEntity.HasComponent<MeshComponent>())
					{
						ResourceManager manager;
						nfdu8char_t* outPath;
						nfdopendialogu8args_t args = { 0 };
						nfdresult_t result = NFD_OpenDialogU8_With(&outPath, &args);
						if (result == NFD_OKAY)
						{
							m_SelectedEntity.AddComponent<MeshComponent>(manager.CreateResource<Model>(outPath));
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
					}
				}

				if (ImGui::MenuItem("Shader Component"))
				{
					m_SelectedEntity.AddComponent<ShaderComponent>(CreateRef<Shader>("Assets/shaders/ModelVertex.glsl", "Assets/shaders/ModelFragment.glsl"));
				}
				ImGui::EndPopup();
			}
			ImGui::SeparatorText("Components");
			DrawComponents();
		}

		ImGui::End();
	}
	void InspectorPanel::SetEntityFocus(Entity& entity)
	{
		m_SelectedEntity = entity;
	}

	static void Draw3VecInput(const char* label, float* values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		ImGui::PushID(label);

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label);
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		//if (ImGui::Button("X", buttonSize))
			//values[0] = resetValue;
		ImGui::ColorButton("v", ImVec4{ 255,0,0,0 });

		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values[0], 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		/*ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		if (ImGui::Button("Y", buttonSize))
			values[1] = resetValue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values[1], 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		if (ImGui::Button("Z", buttonSize))
			values[2] = resetValue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values[2], 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();*/

		ImGui::Columns(1);

		ImGui::PopID();
	}


	void InspectorPanel::DrawComponents()
	{
		// Crear estilo personalizado
		DrawComponent<IDComponent>("ID Component",
			[](auto& component)
			{
				ImGui::Text(std::to_string(component.Id).c_str());
			});
		DrawComponent<TagComponent>("Tag Component",
			[](auto& component)
			{
				ImGui::InputText("Name", &component.Tag);
			});
		DrawComponent<CameraComponent>("Camera Component",
			[](auto& component)
			{
				// Selector de color RGB
				ImGui::ColorEdit4("Color RGB", &component.Color[0]);
				ImGui::InputFloat("Fov", &component.Fov);
				ImGui::Checkbox("Principal Camera", &component.IsPrincipalCamera);
			});
		DrawComponent<TransformComponent>("Transform Component",
			[](auto& component)
			{
				//ImGui::Text("Translation:");
				//Draw3VecInput("Translation", &component.Translation[0]);
				ImGui::InputFloat3("Position", &component.Translation[0]);
				ImGui::Text("Rotation:");
				ImGui::InputFloat3("Rotation", &component.Rotation[0]);
				ImGui::Text("Scale:");
				ImGui::InputFloat3("Scale", &component.Scale[0]);
			});
		DrawComponent<MeshComponent>("Mesh Component",
			[](auto& component)
			{
				if (component.ptr_Model)
				{
					auto& path = component.ptr_Model->GetPath();
					ImGui::Text("Path: %s", path.c_str());
					ImGui::Text("Referencias: %i", component.ptr_Model.use_count());
				}
				else
				{
					ImGui::Text("Path: %s", "");
					ImGui::Text("Referencias: %i", -1);
				}
			});
	}

}