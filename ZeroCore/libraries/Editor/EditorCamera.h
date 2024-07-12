#pragma once
#include <GLGraphics/Camera.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Core/Input.h>

namespace Zero
{
	enum class CameraState
	{
		FREE_CAMERA,
		ORBIT_CAMERA
	};

	class EditorCamera : public Camera
	{
	public:
		EditorCamera(const glm::mat4& projection = {0}, const glm::mat4& view = { 0 })
			: Camera(projection, view), m_Projection(projection), m_View(view)
		{
			m_LastMousePosition = Input::GetMousePosition();
		}

		void Update(float deltaTime);
		void SetPerspectiveProjection(float fovy, float zNear, float zFar);
		void CalculateProjection();
		void CalculateView();
		void SetViewportSize(uint32_t width, uint32_t height);

		glm::mat4 GetProjection() override { return m_Projection; }
		void SetProjection(const glm::mat4& projection) { m_Projection = projection; }
		glm::mat4 GetView() override { return m_View; }
		void SetView(const glm::mat4& view) { m_View = view; }

		void SetPosition(const glm::vec3& position) { m_Position = position; }
		glm::vec3 GetPosition() const { return m_Position; }

		void SetRotation(const glm::vec3& rotation) { m_Rotation = rotation; }
		glm::vec3 GetRotation() const { return m_Rotation; }

		void SetCameraState(CameraState state) { m_CameraState = state; }
		CameraState GetCameraState() const { return m_CameraState; }

		void SetOrbitCenter(const glm::vec3& center) { m_OrbitCenter = center; }
		glm::vec3 GetOrbitCenter() const { return m_OrbitCenter; }

		void SetOrbitRadius(float radius) { m_OrbitRadius = radius; }
		float GetOrbitRadius() const { return m_OrbitRadius; }

	private:
		void HandleMouseInput();
		void HandleKeyboardInput(float deltaTime);

		glm::mat4 m_Projection;
		glm::mat4 m_View;
		float m_Aspect;
		float m_PerspectiveFOV = glm::radians(45.0f);
		float m_PerspectiveNear = 0.01f, m_PerspectiveFar = 1000.0f;

		glm::vec3 m_Position{ 10, 10, 10 };
		glm::vec3 m_Rotation{ 0, 0, 0 };  // En radianes
		glm::vec3 m_OrbitCenter{ 0, 0, 0 };
		float m_OrbitRadius = 10.0f;

		glm::vec3 m_Front{ 0,0,-1 };
		glm::vec3 m_Right{ 1,0,0 };
		CameraState m_CameraState = CameraState::ORBIT_CAMERA;

		glm::vec2 m_LastMousePosition;
		float m_MouseSensitivity = 0.1f;
		float m_MoveSpeed = 5.0f;
		float m_ZoomSpeed = 1.0f;
	};
}