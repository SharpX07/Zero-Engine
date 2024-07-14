#include "PreviewPanel.h"
#include <Scene/Components.h>
#include <Modules/Renderer.h>
#include <GLGraphics/FBO.h>
#include <Core/Aliases.h>
#include <Scene/Entity.h>

namespace Zero
{
	PreviewPanel::PreviewPanel()
	{
		FramebufferConfiguration config;
		config.Height = 500;
		config.Width = 500;
		config.Formats = {FrameBufferFormat::RGBA8,FrameBufferFormat::DEPTH24};
		m_FBO.Create(config);
	}
	void PreviewPanel::OnRender()
    {
		
        if(m_FocusedScene->GetPrincipalCamera())
        {
			ImGui::Begin("Preview Window");
			ImVec2 pos = ImGui::GetCursorScreenPos();
			const float window_width = ImGui::GetContentRegionAvail().x;
			const float window_height = ImGui::GetContentRegionAvail().y;
			//m_FBO.RescaleFramebuffer(window_width, window_height);
			auto tempCamera = m_FocusedScene->GetPrincipalCamera()->GetComponent<CameraComponent>();

			tempCamera.camera->SetViewportSize(window_width, window_height);

			m_FBO.Bind();
			Renderer::Clear({ 0,0,0,255 });
			Renderer::SetViewport(0, 0, window_width, window_height);

			Renderer::RenderOnRuntime(*m_FocusedScene);
			m_FBO.UnBind();
			
			ImGui::GetWindowDrawList()->AddImage(
				(void*)m_FBO.GetColorTexture(0),
				ImVec2(pos.x, pos.y),
				ImVec2(pos.x + window_width, pos.y + window_height),
				ImVec2(0, 1),
				ImVec2(1, 0)
			);
			ImGui::End();
        }else
		{
			ImGui::Begin("Preview Window");
			ImVec2 pos = ImGui::GetCursorScreenPos();
			const float window_width = ImGui::GetContentRegionAvail().x;
			const float window_height = ImGui::GetContentRegionAvail().y;

			// Dibujar un rectángulo negro
			ImGui::GetWindowDrawList()->AddRectFilled(
				ImVec2(pos.x, pos.y),
				ImVec2(pos.x + window_width, pos.y + window_height),
				IM_COL32(0, 0, 0, 255) // Color negro
			);

			ImGui::End();
		}
    }
    void PreviewPanel::SetSceneFocus(Ref<Scene> scene)
    {
		m_FocusedScene = scene;
    }
}