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
			scene.GetRegistry().emplace_or_replace<TransformComponent>(entity, transform.Translation,
				transform.Rotation, transform.Scale);
			scene.GetRegistry().emplace_or_replace<CameraComponent>(entity,camera);
		}
	}
}
