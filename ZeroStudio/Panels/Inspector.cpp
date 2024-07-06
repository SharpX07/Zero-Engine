#include "Inspector.h"
#include <Scene/Components.h>
#include <imgui_stdlib.h>

namespace Zero
{
	void InspectorPanel::OnRender()
	{
		ImGui::Begin("Inspector");
		if (m_SelectedEntity.IsValid())
		{
			DrawComponents();
		}
		ImGui::End();
	}
	void InspectorPanel::SetEntityFocus(Entity& entity)
	{
		m_SelectedEntity = entity;
	}

	void InspectorPanel::DrawComponents()
	{
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
			});
		DrawComponent<TransformComponent>("Transform Component",
			[](auto& component)
			{
				ImGui::Text("Translation:");
				ImGui::InputFloat3("Translation", &component.Translation[0]);
				ImGui::Text("Rotation:");
				ImGui::InputFloat3("Rotation", &component.Rotation[0]);
				ImGui::Text("Scale:");
				ImGui::InputFloat3("Scale", &component.Scale[0]);
			});
		DrawComponent<MeshComponent>("Mesh Component",
			[](auto& component)
			{
				auto& path = component.ptr_Model->GetPath();
				ImGui::Text("Path: %s", path.c_str());
			});
	}

}