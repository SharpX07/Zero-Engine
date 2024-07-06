#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include "SceneCamera.h"
#include <ResourceManagement/Model.h>
#include <Core/Aliases.h>

namespace Zero
{
	struct IDComponent
	{
		std::string Tag;

		IDComponent() = default;
		IDComponent(const IDComponent&) = default;
		IDComponent(const std::string& tag)
			: Tag(tag) {}
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
		glm::vec3 Scale;
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

	struct CameraComponent
	{
		Ref<SceneCamera> camera;
		glm::vec4 Color{ 0,0,0,0 };
		float Fov = 90;
		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	struct MeshComponent
	{
		std::shared_ptr<Model> ptr_Model;

	};

	struct ShaderComponent
	{
		Shader& Shader;
	};
}