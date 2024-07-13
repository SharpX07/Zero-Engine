#include "EditorViewPanel.h"
#include <Scene/Components.h>
#include <Modules/Renderer.h>
#include <GLGraphics/FBO.h>
#include <Core/Aliases.h>


namespace Zero
{

	EditorViewPanel::EditorViewPanel()
	{
		InitializeShader();
		InitializeGeometry();
		InitializeCamera();
	}

	void EditorViewPanel::OnRender()
	{
		DrawView();

		m_FBO.Bind();
		Renderer::Clear({ 0,0,0 ,255 });
		Renderer::RenderOnDebug(m_FocusedScene, m_EditorCamera);
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
			(void*)m_FBO.GetColorTexture(),
			ImVec2(windowPosition.x, windowPosition.y),
			ImVec2(windowPosition.x + window_width, windowPosition.y + window_height),
			ImVec2(0, 1),
			ImVec2(1, 0)
		);
		ImVec2 mousePos = ImGui::GetMousePos();
		HandleMousePick(mousePos.x, mousePos.y);
		ImGui::End();
	}

	void EditorViewPanel::SetSceneFocus(Ref<Scene> scene)
	{
		m_FocusedScene = scene;
	}

	void EditorViewPanel::HandleMousePick(int mouseX, int mouseY)
	{
		// Asegúrate de que el clic está dentro de la ventana del editor
		ImVec2 windowPos = ImGui::GetCursorScreenPos();
		ImVec2 windowSize = ImGui::GetContentRegionAvail();

		if (mouseX >= windowPos.x && mouseX < windowPos.x + windowSize.x &&
			mouseY >= windowPos.y && mouseY < windowPos.y + windowSize.y)
		{
			// Ajusta las coordenadas del mouse al espacio de la textura
			int textureX = mouseX - windowPos.x;
			int textureY = windowSize.y - (mouseY - windowPos.y);

			// Lee el color del píxel
			m_FBO.Bind();
			unsigned char pixel[3];
			glReadPixels(textureX, textureY, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
			m_FBO.UnBind();
			ZERO_CORE_LOG_DEBUG("{0},{1},{2}", pixel[0], pixel[1], pixel[2]);

			// Convierte el color a ID
			uint64_t id = (static_cast<uint64_t>(pixel[0]) << 16) |
				(static_cast<uint64_t>(pixel[1]) << 8) |
				(static_cast<uint64_t>(pixel[2]));
			ZERO_CORE_LOG_DEBUG("id:{0}", id);
			// Busca la entidad correspondiente
			auto entity = m_FocusedScene->GetEntityByUUID(id);
			if (entity)
			{
				m_SelectedEntity = entity;
				// La entidad fue seleccionada
				// Aquí puedes hacer lo que necesites con la entidad seleccionada
				// Por ejemplo, mostrar sus propiedades, resaltarla, etc.
				std::cout << "Entidad seleccionada: " << id << std::endl;
			}
		}
	}
}