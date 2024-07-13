#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include "SceneCamera.h"
#include <ResourceManagement/Model.h>
#include <Core/Aliases.h>
#include <Core/UUID.h>

namespace Zero
{
	struct IDComponent
	{
		UUID Id;

		IDComponent() = default;
		IDComponent(const IDComponent&) = default;
		IDComponent(const UUID Id)
			: Id(Id) {}
	};

	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag) {}
		void Reset()
		{
			Tag = std::string();
		}
	};

	struct TransformComponent
	{
		glm::vec3 Translation;
		glm::vec3 Rotation;
		glm::vec3 Scale{ 1.0f };
		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		glm::mat4 GetTransform() const
		{
			glm::mat4 rotation = glm::mat4(glm::quat(Rotation));

			return glm::translate(glm::mat4(1.0f), Translation)
				* glm::scale(glm::mat4(1.0f), Scale)
				* rotation;
		}
		void Reset()
		{
			Translation = glm::vec3(0.0f);
			Rotation = glm::vec3(0.0f);
			Scale = glm::vec3(1.0f);
		}
	};

	class SceneCamera;

	enum class ProyectionType
	{
		PERSPECTIVE_PROYECTION,
		ORTHOGONAL_PROYECTION
	};

	struct CameraComponent
	{
		Ref<SceneCamera> camera;
		glm::vec4 Color{ 0,0,0,1.0f };
		float Fov = 90.0f;
		bool IsPrincipalCamera = false;

		void Reset()
		{
			Fov = 90.0f;
			Color = glm::vec4{ 0,0,0,1.0f };
			camera.reset();
			camera = CreateRef<SceneCamera>(glm::mat4(1.0f), glm::mat4(1.0f));
		}
		//CameraComponent() = default;
		//CameraComponent(const CameraComponent&) = default;
	};

	struct MeshComponent
	{
		Ref<Model> ptr_Model;
		void Reset()
		{
			ptr_Model.reset();
		}
	};

	struct ShaderComponent
	{
		Ref<Shader> Shader;
		void Reset()
		{
			Shader.reset();
		}
	};
}