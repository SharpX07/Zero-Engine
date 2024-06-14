#include "CameraUpdater.h"
#include <Scene/Components.h>
namespace Zero
{
	void CameraSystem::UpdateCameras(Scene& scene)
	{
		auto view = scene.GetAllEntitiesWith<CameraComponent, TransformComponent>();
		for (auto entity : view)
		{
			CameraComponent& camera = view.get<CameraComponent>(entity);
			TransformComponent& transform = view.get<TransformComponent>(entity);
			camera.camera.SetView(transform.GetTransform());

		}
	}
}
