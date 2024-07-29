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
#include <Core/Timer.h>
#include <Core/Application.h>
#include <ResourceManagement/Billboard.h>
namespace Zero
{
	Ref<Model> Renderer::m_BoxModel;

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

	void Renderer::RenderModel(MeshComponent model, Ref<Shader> shaderInUse)
	{
		for (const auto& mesh : model.ptr_Model->GetMeshes()) {
			auto material = mesh.Material;
			auto properties = material.GetProperties();
			shaderInUse->setVec3("albedo", properties.Diffuse);
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
		static Ref<Shader> EditorEntityShader = ResourceManager::GetInstance().CreateResource<Shader>("Assets/Core/Shaders/EntityEditor.glsl");
		static Billboard cameraBillboard(ResourceManager::GetInstance().CreateResource<GLTexture>("Assets/Core/Textures/camera.png"));
		static Billboard lightBillboard(ResourceManager::GetInstance().CreateResource<GLTexture>("Assets/Core/Textures/light.png"));


		for (auto enttValue : scene->GetAllEntitiesWith < TransformComponent, CameraComponent>())
		{
			Entity entity = { enttValue,scene.get() };

			cameraBillboard.GetShader()->Use();
			cameraBillboard.GetShader()->setMat4("projection", editorCamera->GetProjection());
			cameraBillboard.GetShader()->setMat4("view", editorCamera->GetView());
			cameraBillboard.GetShader()->setVec3("cameraPosition", editorCamera->GetPosition());
			cameraBillboard.GetShader()->setVec3("u_BillboardPos", entity.GetComponent<TransformComponent>().Translation);
			cameraBillboard.GetShader()->setVec2("u_BillboardSize", glm::vec2(1.0f));
			cameraBillboard.GetShader()->setUInt("u_EntityId", (uint32_t)entity);

			cameraBillboard.Bind();
			glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(6), GL_UNSIGNED_INT, 0);
			glActiveTexture(GL_TEXTURE0);
		}

		for (auto enttValue : scene->GetAllEntitiesWith<TransformComponent, MeshComponent>())
		{
			Entity entity = { enttValue,scene.get() };
			MeshComponent& model = entity.GetComponent<MeshComponent>();
			if (!model.ptr_Model) continue;

			Ref<Shader> shaderInUse = EditorEntityShader;
			if (entity.HasComponent<ShaderComponent>())
				shaderInUse = entity.GetComponent<ShaderComponent>().Shader;

			shaderInUse->Use();
			shaderInUse->setMat4("projection", editorCamera->GetProjection());
			shaderInUse->setMat4("view", editorCamera->GetView());
			shaderInUse->setMat4("model", entity.GetComponent<TransformComponent>().GetTransform());
			shaderInUse->setVec3("cameraPosition", editorCamera->GetPosition());
			shaderInUse->setVec3("lightPosition", editorCamera->GetPosition());
			shaderInUse->setUInt("u_EntityId", (uint32_t)entity);
			shaderInUse->setVec2("u_Resolution", Application::GetInstance()->GetResolution());
			RenderModel(model, shaderInUse);
			if (entity == EntitySelector::GetEntitySelected())
			{
				RenderBox(entity, editorCamera);
			}
		}
	}

	void Renderer::RenderBox(Entity& entity, Scope<EditorCamera>& editorCamera)
	{
		static ShaderParser parser;
		static Ref<Shader> FlatShader = parser.GenerateShader("Assets/Shaders/FlatColor.glsl");
		glm::mat4 transform = entity.GetComponent<TransformComponent>().GetTransform();
		MeshComponent& model = entity.GetComponent<MeshComponent>();
		auto boundingBox = model.ptr_Model->GetBoundingBox();

		glm::vec3 center = (boundingBox.min + boundingBox.max) * 0.5f;
		glm::vec3 scale = boundingBox.max - boundingBox.min;
		glm::mat4 bbTransform = glm::mat4(1.0f);
		bbTransform = glm::translate(bbTransform, center);
		bbTransform = glm::scale(bbTransform, scale);

		FlatShader->Use();
		FlatShader->setMat4("model", transform * bbTransform);
		FlatShader->setMat4("projection", editorCamera->GetProjection());
		FlatShader->setMat4("view", editorCamera->GetView());
		FlatShader->setVec3("cameraPosition", editorCamera->GetPosition());
		FlatShader->setVec3("u_FlatColor", glm::vec3(137, 87, 17));
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Modo de líneas
		m_BoxModel->GetMeshes().at(0).m_VAO->Bind();
		glLineWidth(2);
		glDrawElements(GL_LINE_STRIP, static_cast<unsigned int>(m_BoxModel->GetMeshes().at(0).VertexIndices.size()), GL_UNSIGNED_INT, 0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Volver al modo de relleno
		glLineWidth(1);
	}
	void Renderer::InitializeRenderer()
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
		Renderer::EnableCapability(GL_DEPTH_TEST);
		Renderer::EnableCapability(GL_BLEND);
		ZERO_CORE_LOG_INFO("Loading default texture");

		m_BoxModel = ResourceManager::GetInstance().CreateResource<Model>("Assets/Models/SimpleCube.obj");
	}
}