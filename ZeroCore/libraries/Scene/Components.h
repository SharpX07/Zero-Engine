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
	};

	struct TransformComponent
	{
		glm::vec3 Translation;
		glm::vec3 Rotation;
		glm::vec3 Scale{1.0f};
		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		glm::mat4 GetTransform() const
		{
			glm::mat4 rotation = glm::mat4(glm::quat(Rotation));

			return glm::translate(glm::mat4(1.0f), Translation)
				* glm::scale(glm::mat4(1.0f), Scale)
				* rotation;
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
		glm::vec4 Color{ 0,0,0,0 };
		float Fov = 90;
		bool IsPrincipalCamera = false;

		//CameraComponent() = default;
		//CameraComponent(const CameraComponent&) = default;
	};

	struct MeshComponent
	{
		Ref<Model> ptr_Model;
	};

	struct ShaderComponent
	{
		Ref<Shader> Shader;
	};
}