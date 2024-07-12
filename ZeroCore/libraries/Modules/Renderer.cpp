#include "Renderer.h"
#include <core/Logger.h>
#include <Scene/Scene.h>
#include <Scene/Entity.h>
#include <Scene/Components.h>
#include <Editor/EditorCamera.h>
#include <Core/Logger.h>
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
		static GLTexture noTextureSample("Preloaded Texture"); // Variable estática local
		// TODO: This would be a principal camera
		CameraComponent& camera = scene.GetPrincipalCamera()->GetComponent<CameraComponent>();
		Renderer::Clear({ camera.Color.r*255.0f ,camera.Color.g * 255.0f ,camera.Color.b * 255.0f ,camera.Color.a * 255.0f });
		auto view = scene.GetAllEntitiesWith<TransformComponent, MeshComponent, ShaderComponent>();
		for (auto entity : view)
		{
			TransformComponent& transform = view.get<TransformComponent>(entity);
			MeshComponent& model = view.get<MeshComponent>(entity);
			if (!model.ptr_Model) return;

			ShaderComponent& shader = view.get<ShaderComponent>(entity);
			
			shader.Shader->Use();
			shader.Shader->setMat4("model", transform.GetTransform());
			shader.Shader->setMat4("projection", camera.camera->GetProjection());
			shader.Shader->setMat4("view", camera.camera->GetView());

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

	void Renderer::RenderOnEditor(Ref<Scene> scene, Scope<EditorCamera>& editorCamera)
	{
		auto view = scene->GetAllEntitiesWith<TransformComponent, MeshComponent, ShaderComponent>();
		for (auto entity : view)
		{
			auto entidad = scene->GetEntityByID(entity);
			TransformComponent& transform = entidad.GetComponent<TransformComponent>();
			MeshComponent& model = entidad.GetComponent<MeshComponent>();
			if (!model.ptr_Model) return;
			ShaderComponent& shader = entidad.GetComponent<ShaderComponent>();
			shader.Shader->Use();
			shader.Shader->setMat4("model", transform.GetTransform());
			shader.Shader->setMat4("projection", editorCamera->GetProjection());
			shader.Shader->setMat4("view", editorCamera->GetView());
			shader.Shader->setVec3("cameraPosition",editorCamera->GetPosition());
			shader.Shader->setVec3("lightPosition", editorCamera->GetPosition());

			for (const auto& mesh : model.ptr_Model->GetMeshes()) {
				auto material = mesh.Material;
				auto properties = material.GetProperties();
				shader.Shader->setVec3("albedo", properties.Albedo);
				shader.Shader->setFloat("metallic", properties.Metallic);
				shader.Shader->setFloat("roughness", properties.Roughness);
				shader.Shader->setBool("hasAlbedoTexture", properties.hasAlbedoTexture);

				for (unsigned int i = 0; i < mesh.Material.GetNumTextures(); i++)
				{
					const auto& texture = mesh.Material.getTextures().at(i);
					if (texture.Type == DIFFUSE)
						texture.GlTexture->Bind(0);
					//if (texture.Type == NORMAL)
						//texture.GlTexture->Bind(1);
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
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
		ZERO_CORE_LOG_INFO("Loading default texture");
	}
}