#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include "SceneCamera.h"
#include <ResourceManagement/Model.h>

namespace Zero
{
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
				* rotation
				* glm::scale(glm::mat4(1.0f), Scale);
		}
	};

	class SceneCamera;

	struct CameraComponent
	{
		SceneCamera camera;
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