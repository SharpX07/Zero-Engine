#include "EditorViewPanel.h"
#include <Scene/Components.h>
#include <Modules/Renderer.h>
#include <GLGraphics/FBO.h>
#include <Core/Aliases.h>


namespace Zero
{

	EditorViewPanel::EditorViewPanel()
	{
		FramebufferConfiguration config;
		config.Height = 500;
		config.Width = 500;
		config.Formats = { FrameBufferFormat::RGBA8,FrameBufferFormat::R32UI,FrameBufferFormat::DEPTH24 };
		m_FBO.Create(config);
		InitializeShader();
		InitializeGeometry();
		InitializeCamera();
	}

	void EditorViewPanel::OnRender()
	{
		DrawView();

		m_FBO.Bind();
		Renderer::Clear({ 0,0,0 ,255 });
		Renderer::RenderOnEditor(m_FocusedScene, m_EditorCamera);
		DrawGrid();
		m_FBO.UnBind();
	}

	void EditorViewPanel::InitializeCamera() {
		m_EditorCamera = CreateScope<EditorCamera>();
		m_EditorCamera->SetPerspectiveProjection(glm::radians(45.0f), 0.1f, 5000.0f);
		m_EditorCamera->SetViewportSize(800, 800);
		m_EditorCamera->CalculateProjection();
		m_EditorCamera->SetPosition({ 4,4,-4 });
	}

	void EditorViewPanel::InitializeShader() {

		m_TestShader = CreateScope<Shader>("Assets/Shaders/Grid.vert", "Assets/Shaders/Grid.frag");
	}

	void EditorViewPanel::InitializeGeometry() {
		std::vector<float> Vertices = { 0.0,0.0,0.0,0.0,0.0,0.0 ,0.0,0.0,0.0 };
		m_VAO = CreateScope<VAO>();
		VertexAttributeLayout layout;
		m_VAO->Bind();
		m_VBO = CreateScope<VBO>(&Vertices[0], Vertices.size() * sizeof(float));
		layout.addAttribute(GL_FLOAT, 3);
		m_VAO->AddBuffer(*m_VBO, layout);
	}

	void EditorViewPanel::DrawGrid()
	{
		m_VAO->Bind();
		m_TestShader->Use();
		m_TestShader->setMat4("view", m_EditorCamera->GetView());
		m_TestShader->setMat4("projection", m_EditorCamera->GetProjection());
		m_TestShader->setVec3("pos", m_EditorCamera->GetPosition() + glm::vec3(0, 1, 0));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		m_VAO->UnBind();
	}

	void EditorViewPanel::DrawView()
	{
		ImGui::Begin("Game Window");
		ImVec2 windowPosition = ImGui::GetCursorScreenPos();
		const float window_width = ImGui::GetContentRegionAvail().x;
		const float window_height = ImGui::GetContentRegionAvail().y;
		Renderer::SetViewport(0, 0, window_width, window_height);
		m_FBO.RescaleFramebuffer(window_width, window_height);
		m_EditorCamera->SetViewportSize(window_width, window_height);
		ImGui::GetWindowDrawList()->AddImage(
			(void*)m_FBO.GetColorTexture(0),
			ImVec2(windowPosition.x, windowPosition.y),
			ImVec2(windowPosition.x + window_width, windowPosition.y + window_height),
			ImVec2(0, 1),
			ImVec2(1, 0)
		);
		HandleMousePick();
		ImGui::End();
	}

	void EditorViewPanel::SetSceneFocus(Ref<Scene> scene)
	{
		m_FocusedScene = scene;
	}

	void EditorViewPanel::HandleMousePick()
	{
		if (!ImGui::IsMouseClicked(ImGuiMouseButton_Left))
		{
			return;
		}
		m_FBO.Bind();
		ImVec2 mousePos = ImGui::GetMousePos();
		ImVec2 windowSize = ImGui::GetContentRegionAvail();
		ImVec2 panelPos = ImGui::GetCursorScreenPos();

		ImVec2 localMousePos = ImVec2(mousePos.x - panelPos.x, mousePos.y - panelPos.y);
		unsigned int pickedID = 0;
		int mouseX = localMousePos.x;
		int mouseY = localMousePos.y;
		int textureX = mouseX;
		int textureY = windowSize.y - mouseY;
		glReadBuffer(GL_COLOR_ATTACHMENT1);
		unsigned int pixel;
		glReadPixels(textureX, textureY, 1, 1, GL_RED_INTEGER, GL_UNSIGNED_INT, &pixel);
		int Checkid = pixel - 1;
		if (Checkid != -1)
			m_SelectedEntity = { (entt::entity)Checkid,m_FocusedScene.get() };
		m_FBO.UnBind();
	}
}