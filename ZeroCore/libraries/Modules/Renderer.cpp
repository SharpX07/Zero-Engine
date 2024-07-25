#include "Renderer.h"
#include <core/Logger.h>
#include <Scene/Scene.h>
#include <Scene/Entity.h>
#include <Scene/Components.h>
#include <Editor/EditorCamera.h>
#include <Core/Logger.h>
#include <GLGraphics/ShaderParser.h>
#include <ResourceManagement/ResourceManager.h>
#include <Modules/EntitySelector.h>
namespace Zero
{
	void Renderer::RenderOnRuntime(Scene& scene)
	{
		// TODO: This would be a principal camera
		CameraComponent& camera = scene.GetPrincipalCamera()->GetComponent<CameraComponent>();
		Renderer::Clear({ camera.Color.r * 255.0f ,camera.Color.g * 255.0f ,camera.Color.b * 255.0f ,camera.Color.a * 255.0f });
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
				//noTextureSample.Bind(0);
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

	float GetCurrentTimeInSeconds() {
		// Obtener el tiempo actual usando el reloj de alta resolución
		auto now = std::chrono::high_resolution_clock::now();

		// Convertir el tiempo a una duración en segundos desde un tiempo inicial (epoch)
		auto duration = std::chrono::duration<float>(now.time_since_epoch());

		// Obtener el valor en segundos
		return duration.count();
	}

	void Renderer::RenderModel(MeshComponent model, Ref<Shader> shaderInUse)
	{
		for (const auto& mesh : model.ptr_Model->GetMeshes()) {
			auto material = mesh.Material;
			auto properties = material.GetProperties();
			shaderInUse->setVec3("albedo", properties.Albedo);
			shaderInUse->setFloat("metallic", properties.Metallic);
			shaderInUse->setFloat("roughness", properties.Roughness);
			shaderInUse->setBool("hasAlbedoTexture", properties.hasAlbedoTexture);

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

	void Renderer::RenderOnEditor(Ref<Scene> scene, Scope<EditorCamera>& editorCamera, Entity selected)
	{
		static Ref<Shader> EditorEntityShader = ResourceManager::GetInstance().CreateResource<Shader>("Assets/Shaders/EntityEditor.glsl");

		auto view = scene->GetAllEntitiesWith<TransformComponent, MeshComponent>();
		for (auto entity : view)
		{
			Entity entidad = { entity,scene.get() };
			MeshComponent& model = entidad.GetComponent<MeshComponent>();
			if (!model.ptr_Model) continue;

			
			Ref<Shader> shaderInUse = EditorEntityShader;
			if (entidad.HasComponent<ShaderComponent>())
				shaderInUse = entidad.GetComponent<ShaderComponent>().Shader;


			shaderInUse->Use();
			shaderInUse->setMat4("model", entidad.GetComponent<TransformComponent>().GetTransform());
			shaderInUse->setMat4("projection", editorCamera->GetProjection());
			shaderInUse->setMat4("view", editorCamera->GetView());
			shaderInUse->setVec3("cameraPosition", editorCamera->GetPosition());
			shaderInUse->setVec3("lightPosition", editorCamera->GetPosition());
			shaderInUse->setFloat("u_Time", GetCurrentTimeInSeconds());
			shaderInUse->setUInt("u_EntityId", (uint32_t)entity);
			if (entidad == EntitySelector::GetEntitySelected())
				shaderInUse->setBool("isMarked", true);
			else
				shaderInUse->setBool("isMarked", false);
			RenderModel(model, shaderInUse);
		}
	}

	void Renderer::RenderOnDebug(Ref<Scene> scene, Scope<EditorCamera>& editorCamera)
	{
		auto view = scene->GetAllEntitiesWith<TransformComponent, MeshComponent, ShaderComponent>();
		static ShaderParser parser;
		static Ref<Shader> shader = parser.GenerateShader("Assets/Shaders/ColorPicking.glsl");
		for (auto entity : view)
		{

			auto entidad = scene->GetEntityByID(entity);
			MeshComponent& model = entidad.GetComponent<MeshComponent>();
			IDComponent& id = entidad.GetComponent<IDComponent>();
			if (!model.ptr_Model) continue;
			shader->Use();
			shader->setUInt("u_EntityId", (uint32_t)entity);
			shader->setMat4("model", entidad.GetComponent<TransformComponent>().GetTransform());
			shader->setMat4("projection", editorCamera->GetProjection());
			shader->setMat4("view", editorCamera->GetView());

			for (const auto& mesh : model.ptr_Model->GetMeshes()) {
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