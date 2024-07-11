#include "EditorViewPanel.h"
#include <Scene/Components.h>
#include <Modules/Renderer.h>
#include <GLGraphics/FBO.h>
#include <Core/Aliases.h>


namespace Zero
{
	void EditorViewPanel::OnRender(EditorCamera& editorCamera)
	{
		// Ventana para mostrar el framebuffer
		ImGui::Begin("Game Window");
		ImVec2 pos = ImGui::GetCursorScreenPos();
		const float window_width = ImGui::GetContentRegionAvail().x;
		const float window_height = ImGui::GetContentRegionAvail().y;
		Renderer::SetViewport(0, 0, window_width, window_height);
		m_FBO.RescaleFramebuffer(window_width, window_height);
		editorCamera.SetViewportSize(window_width, window_height);
		ImGui::GetWindowDrawList()->AddImage(
			(void*)m_FBO.GetColorTexture(),
			ImVec2(pos.x, pos.y),
			ImVec2(pos.x + window_width, pos.y + window_height),
			ImVec2(0, 1),
			ImVec2(1, 0)
		);
		ImGui::End();

		m_FBO.Bind();
		//auto cameraCmp = Camera.GetComponent<CameraComponent>();
		//Renderer::Clear({ cameraCmp.Color.r * 255.0f,cameraCmp.Color.g * 255.0f,cameraCmp.Color.b * 255.0f ,cameraCmp.Color.a * 255.0f });
		Renderer::Clear({ 0,0,0 ,255 });
		Renderer::RenderOnEditor(m_FocusedScene, editorCamera);
		m_TestShader->Use();
		m_TestShader->setMat4("view", editorCamera.GetView());
		m_TestShader->setMat4("projection", editorCamera.GetProjection());
		m_TestShader->setVec3("pos", editorCamera.GetPosition());
		glDrawArrays(GL_TRIANGLES, 0, 6);
		m_FBO.UnBind();
	}
	void EditorViewPanel::SetSceneFocus(Ref<Scene> scene)
	{
		m_FocusedScene = scene;
	}
}