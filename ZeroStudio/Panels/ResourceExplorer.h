#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <string>
#include <Core/Aliases.h>
#include <ResourceManagement/Resource.h>
#include <Editor/Panel.h>

namespace Zero
{
	class ResourcesExplorer : public Panel
	{
	public:
		void OnRender() override;
		//void GetResourceFocus();
	private:
		void DrawResources();
		void DrawResourceThumbnail(const Ref<Resource>& resource, const ImVec2& size);
		Ref<Resource> m_SelectedResource;
	};
}
