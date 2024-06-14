#include "Renderer.h"
#include <core/Logger.h>
#include <Scene/Scene.h>
#include <Scene/Components.h>
#include <Editor/EditorCamera.h>
namespace Zero
{

	void Renderer::Render(Model& model, Shader& shader)
	{
		for (const auto& mesh : model.GetMeshes()) {
			for (unsigned int i = 0; i < mesh.Material.GetNumTextures(); i++)
			{
				const auto& texture = mesh.Material.getTextures().at(i);
				if (texture.Type == DIFFUSE)
					texture.GlTexture->Bind(0);
				if (texture.Type == NORMAL)
					texture.GlTexture->Bind(1);
			}
			mesh.m_VAO->Bind();
			glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(mesh.VertexIndices.size()), GL_UNSIGNED_INT, 0);
			glActiveTexture(GL_TEXTURE0);
		}
	}

	void Renderer::RenderOnRuntime(Scene& scene)
	{
		static GLTexture noTextureSample("Assets/Textures/sm.png"); // Variable estática local

		auto view = scene.GetAllEntitiesWith<TransformComponent, MeshComponent, ShaderComponent>();
		for (auto entity : view)
		{
			TransformComponent& transform = view.get<TransformComponent>(entity);
			MeshComponent& model = view.get<MeshComponent>(entity);
			if (!model.ptr_Model) return;

			ShaderComponent& shader = view.get<ShaderComponent>(entity);
			SceneCamera& camera = scene.GetRegistry().get<CameraComponent>(scene.GetPrincipalCamera()).camera;
			shader.Shader.Use();
			shader.Shader.setMat4("model", transform.GetTransform());
			shader.Shader.setMat4("projection", camera.GetProjection());
			shader.Shader.setMat4("view", camera.GetView());

			for (const auto& mesh : model.ptr_Model->GetMeshes()) {
				noTextureSample.Bind(0);
				for (unsigned int i = 0; i < mesh.Material.GetNumTextures(); i++)
				{
					const auto& texture = mesh.Material.getTextures().at(i);
					if (texture.Type == DIFFUSE)
						texture.GlTexture->Bind(0);
					if (texture.Type == NORMAL)
						texture.GlTexture->Bind(1);
				}
				mesh.m_VAO->Bind();
				glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(mesh.VertexIndices.size()), GL_UNSIGNED_INT, 0);
				glActiveTexture(GL_TEXTURE0);
			}
		}
	}

	void Renderer::RenderOnEditor(Scene& scene, EditorCamera& editorCamera)
	{
		static GLTexture noTextureSample("Assets/Textures/sm.png"); // Variable estática local

		auto view = scene.GetAllEntitiesWith<TransformComponent, MeshComponent, ShaderComponent>();
		for (auto entity : view)
		{
			TransformComponent& transform = view.get<TransformComponent>(entity);
			MeshComponent& model = view.get<MeshComponent>(entity);
			if (!model.ptr_Model) return;

			ShaderComponent& shader = view.get<ShaderComponent>(entity);
			shader.Shader.Use();
			shader.Shader.setMat4("model", transform.GetTransform());
			shader.Shader.setMat4("projection", editorCamera.GetProjection());
			shader.Shader.setMat4("view", editorCamera.GetView());

			for (const auto& mesh : model.ptr_Model->GetMeshes()) {
				noTextureSample.Bind(0);
				for (unsigned int i = 0; i < mesh.Material.GetNumTextures(); i++)
				{
					const auto& texture = mesh.Material.getTextures().at(i);
					if (texture.Type == DIFFUSE)
						texture.GlTexture->Bind(0);
					if (texture.Type == NORMAL)
						texture.GlTexture->Bind(1);
				}
				mesh.m_VAO->Bind();
				glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(mesh.VertexIndices.size()), GL_UNSIGNED_INT, 0);
				glActiveTexture(GL_TEXTURE0);
			}
		}
	}

	void Renderer::Render(Shader& shader)
	{

	}

	void Renderer::InitializeRenderer()
	{
		//noTextureSample = GLTexture("Assets/Core/Textures/missing_texture.jpg");
		//noTextureSample = std::make_unique<GLTexture>("Assets/Core/Textures/missing_texture.jpg");
		ZERO_CORE_LOG_INFO("Cargando Textura Predefinida");
	}
}