#include "PreviewPanel.h"
#include <Scene/Components.h>
#include <Modules/Renderer.h>
#include <GLGraphics/FBO.h>
#include <Core/Aliases.h>

namespace Zero
{
    void PreviewPanel::OnRender()
    {
		
        if(m_SelectedEntity.HasComponent<CameraComponent>())
        {
			


			ImGui::Begin("Preview Window");
			ImVec2 pos = ImGui::GetCursorScreenPos();
			const float window_width = ImGui::GetContentRegionAvail().x;
			const float window_height = ImGui::GetContentRegionAvail().y;
			fbo.RescaleFramebuffer(window_width, window_height);
			auto& tempCamera = m_SelectedEntity.GetComponent<CameraComponent>();
			tempCamera.camera->SetViewportSize(window_width, window_height);
			
			fbo.Bind();
			Renderer::Clear({ 0,0,0,255 });
			Renderer::SetViewport(0, 0, window_width, window_height);

			Renderer::RenderOnRuntime(*m_SelectedEntity.GetScene());
			glDrawArrays(GL_TRIANGLES, 0, 6);
			fbo.UnBind();
			
			ImGui::GetWindowDrawList()->AddImage(
				(void*)fbo.GetColorTexture(),
				ImVec2(pos.x, pos.y),
				ImVec2(pos.x + window_width, pos.y + window_height),
				ImVec2(0, 1),
				ImVec2(1, 0)
			);
			ImGui::End();
        }
    }
    void PreviewPanel::SetCameraFocus(Entity& entity)
    {
        m_SelectedEntity = entity;
    }
}