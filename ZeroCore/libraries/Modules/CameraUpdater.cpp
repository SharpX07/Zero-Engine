#include "CameraUpdater.h"
#include <Scene/Components.h>
#include <Core/Aliases.h>
#include <Scene/Entity.h>
namespace Zero
{
	void CameraSystem::UpdateCameras(Scene& scene)
	{
		auto view = scene.GetAllEntitiesWith<CameraComponent, TransformComponent>();
		for (auto entity : view)
		{
			CameraComponent& camera = view.get<CameraComponent>(entity);
			TransformComponent& transform = view.get<TransformComponent>(entity);
			
			camera.camera->SetView(transform.GetTransform());
			camera.camera->SetFOV(glm::radians(camera.Fov));
			if (camera.IsPrincipalCamera)
				scene.SetPrincipalCamera(CreateRef<Entity>(scene.GetEntityByID(entity)));
		}
	}
}
