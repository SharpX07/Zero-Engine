#include "EditorViewPanel.h"
#include <Scene/Components.h>
#include <Modules/Renderer.h>
#include <GLGraphics/FBO.h>
#include <Core/Aliases.h>


namespace Zero
{

	EditorViewPanel::EditorViewPanel()
	{
		m_TestShader = CreateScope<Shader>("Assets/Shaders/Grid.vert", "Assets/Shaders/Grid.frag");
		std::vector<float> Vertices = { 0.0,0.0,0.0,0.0,0.0,0.0 ,0.0,0.0,0.0 };
		m_VAO = CreateScope<VAO>();
		VertexAttributeLayout layout;
		m_VAO->Bind();
		m_VBO = CreateScope<VBO>(&Vertices[0], Vertices.size() * sizeof(float));
		layout.addAttribute(GL_FLOAT, 3);
		m_VAO->AddBuffer(*m_VBO, layout);
		m_EditorCamera = CreateScope<EditorCamera>();
		m_EditorCamera->SetPerspectiveProjection(glm::radians(45.0f), 0.1, 5000.0);
		m_EditorCamera->SetViewportSize(800, 800);
		m_EditorCamera->CalculateProjection();
		m_EditorCamera->SetPosition({ 0,0,-4 });
	}

	void EditorViewPanel::OnRender()
	{

		// Ventana para mostrar el framebuffer
		ImGui::Begin("Game Window");
		ImVec2 pos = ImGui::GetCursorScreenPos();
		const float window_width = ImGui::GetContentRegionAvail().x;
		const float window_height = ImGui::GetContentRegionAvail().y;
		Renderer::SetViewport(0, 0, window_width, window_height);
		m_FBO.RescaleFramebuffer(window_width, window_height);
		m_EditorCamera->SetViewportSize(window_width, window_height);
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
		Renderer::RenderOnEditor(m_FocusedScene, m_EditorCamera);
		m_VAO->Bind();
		m_TestShader->Use();
		m_TestShader->setMat4("view", m_EditorCamera->GetView());
		m_TestShader->setMat4("projection", m_EditorCamera->GetProjection());
		m_TestShader->setVec3("pos", m_EditorCamera->GetPosition() + glm::vec3(0, 1, 0));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		m_VAO->UnBind();
		m_FBO.UnBind();
	}
	void EditorViewPanel::SetSceneFocus(Ref<Scene> scene)
	{
		m_FocusedScene = scene;
	}
}