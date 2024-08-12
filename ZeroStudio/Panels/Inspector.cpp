#include "Inspector.h"
#include <Scene/Components.h>
#include <imgui_stdlib.h>
#include <imgui_internal.h>
#include <ResourceManagement/ResourceManager.h>
#include <Modules/EntitySelector.h>
#include <nfd.h>

namespace Zero
{
	void InspectorPanel::OnRender()
	{
		ImGui::Begin("Inspector");
		// Obtener una referencia al estilo global
		ImGuiStyle& style = ImGui::GetStyle();

		// Cambiar el color de fondo de las ventanas
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.09f, 0.11f, 0.149f, 1.0f); // Gris oscuro

		if (EntitySelector::GetEntitySelected().IsValid())
		{
			m_SelectedEntity = EntitySelector::GetEntitySelected();
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
						nfdu8char_t* outPath;
						nfdopendialogu8args_t args = { 0 };
						nfdresult_t result = NFD_OpenDialogU8_With(&outPath, &args);
						if (result == NFD_OKAY)
						{
							m_SelectedEntity.AddComponent<MeshComponent>(ResourceManager::GetInstance().CreateResource<Model>(outPath));
							NFD_FreePathU8(outPath);
						}
						else if (result == NFD_CANCEL)
							ZERO_APP_LOG_INFO("Model loader cancelled");
						else
							ZERO_APP_LOG_ERROR(NFD_GetError());
					}
				}
				if (ImGui::MenuItem("Shader Component"))
				{
					nfdu8char_t* outPath;
					nfdopendialogu8args_t args = { 0 };
					nfdresult_t result = NFD_OpenDialogU8_With(&outPath, &args);
					if (result == NFD_OKAY)
					{
						m_SelectedEntity.AddComponent<ShaderComponent>(ResourceManager::GetInstance().CreateResource<Shader>(outPath));
						NFD_FreePathU8(outPath);
					}
					else if (result == NFD_CANCEL)
						ZERO_APP_LOG_INFO("Shader loader cancelled");
					else
						ZERO_APP_LOG_ERROR(NFD_GetError());
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
		if (entity.IsValid())
			m_SelectedEntity = entity;
	}

	static void Draw3VecInput(const char* label, float* values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		ImGui::PushID(label);
		ImGui::GetStyle().Colors[ImGuiCol_Separator] = ImVec4(0, 0, 0, 0);
		ImGui::Columns(2, "locations", false);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label);
		ImGui::NextColumn();

		float elementWidth = (ImGui::GetContentRegionAvail().x) / 3.0f;

		auto CustomDragFloat = [](const char* label, float& value, ImU32 lineColor, float width, float lineThickness = 1.0f) {
			ImGui::BeginGroup();
			float labelWidth = ImGui::CalcTextSize(label).x;
			float dragFloatWidth = width * 0.7f - labelWidth - ImGui::GetStyle().ItemSpacing.x;

			ImGui::Text(label);
			ImGui::SameLine();
			ImGui::SetNextItemWidth(dragFloatWidth);
			ImGui::DragFloat(("##" + std::string(label)).c_str(), &value, 0.1f, 0.0f, 0.0f, "%.2f");

			ImVec2 p = ImGui::GetItemRectMin();
			ImVec2 pMax = ImGui::GetItemRectMax();
			ImGui::GetWindowDrawList()->AddLine(
				ImVec2(p.x, pMax.y),
				ImVec2(pMax.x, pMax.y),
				lineColor, lineThickness
			);
			ImGui::EndGroup();
			};

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(ImGui::GetStyle().ItemSpacing.x, 0));

		CustomDragFloat("X", values[0], ImGui::GetColorU32(ImVec4(1.0f, 0.0f, 0.0f, 1.0f)), elementWidth);
		ImGui::SameLine();
		CustomDragFloat("Y", values[1], ImGui::GetColorU32(ImVec4(0.0f, 1.0f, 0.0f, 1.0f)), elementWidth);
		ImGui::SameLine();
		CustomDragFloat("Z", values[2], ImGui::GetColorU32(ImVec4(0.0f, 0.0f, 1.0f, 1.0f)), elementWidth);

		ImGui::PopStyleVar();

		// Reset button
		ImGui::SameLine();
		if (ImGui::Button("Reset")) {
			for (int i = 0; i < 3; i++) {
				values[i] = resetValue;
			}
		}

		ImGui::Columns(1);
		ImGui::PopID();
	}

	void InspectorPanel::DrawComponents()
	{
		// Crear estilo personalizado

		DrawComponent<IDComponent>("ID Component",
			[](auto& component)
			{
 				ImGui::InputText("ID", &std::to_string(component.Id), ImGuiInputTextFlags_ReadOnly);
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
				Draw3VecInput("Position", &component.Translation[0], 0.0f);
				Draw3VecInput("Rotation", &component.Rotation[0], 0.0f);
				Draw3VecInput("Scale", &component.Scale[0], 1.0f);
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
		DrawComponent<ShaderComponent>("Shader Component",
			[](auto& component)
			{
				if (component.Shader)
				{
					ImGui::Text("ProgramValue: %i", component.Shader->m_ShaderProgram);
				}
			});
	}

}