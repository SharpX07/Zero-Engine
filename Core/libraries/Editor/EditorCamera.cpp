#include "EditorCamera.h"
#include <Core/Assert.h>
#include <Events/MouseEvents.h>
namespace Zero
{
	void EditorCamera::Update(float deltaTime)
	{
		HandleMouseInput();
		HandleKeyboardInput(deltaTime);
		CalculateView();
	}
	void EditorCamera::SetPerspectiveProjection(float fovy, float zNear, float zFar)
	{
		m_PerspectiveFOV = fovy;
		m_PerspectiveNear = zNear;
		m_PerspectiveFar = zFar;
	}

	void EditorCamera::CalculateProjection()
	{
		m_Projection = glm::perspective(m_PerspectiveFOV, m_Aspect, m_PerspectiveNear, m_PerspectiveFar);
	}
	void EditorCamera::HandleMouseInput()
	{
		glm::vec2 currentMousePos;
		glm::vec2 delta;
		static float phi = 0;
		static float theta = 0;
		currentMousePos = Input::GetMousePosition();
		delta = currentMousePos - m_LastMousePosition;
		m_LastMousePosition = currentMousePos;
		if(Input::MouseScrolled())
		{
			m_OrbitRadius -= Input::GetMouseScrollDelta();
			theta -= glm::radians(delta.x * m_MouseSensitivity);
			phi += glm::radians(delta.y * m_MouseSensitivity);

			float sinTheta = glm::sin(theta);
			float cosTheta = glm::cos(theta);
			float sinPhi = glm::sin(phi);
			float cosPhi = glm::cos(phi);

			m_Position.x = m_OrbitCenter.x + m_OrbitRadius * sinTheta * cosPhi;
			m_Position.y = m_OrbitCenter.y + m_OrbitRadius * sinPhi;
			m_Position.z = m_OrbitCenter.z + m_OrbitRadius * cosTheta * cosPhi;
		}
		if (Input::MousePressed(Zero::MouseCode(Zero::MouseButton::LEFT)))
		{
			if (m_CameraState == CameraState::FREE_CAMERA)
			{
				m_Rotation.y += delta.x * m_MouseSensitivity;
				m_Rotation.x += delta.y * m_MouseSensitivity;
			}
			else
			{
				
				theta -= glm::radians(delta.x * m_MouseSensitivity);
				phi += glm::radians(delta.y * m_MouseSensitivity);

				float sinTheta = glm::sin(theta);
				float cosTheta = glm::cos(theta);
				float sinPhi = glm::sin(phi);
				float cosPhi = glm::cos(phi);

				m_Position.x = m_OrbitCenter.x + m_OrbitRadius * sinTheta * cosPhi;
				m_Position.y = m_OrbitCenter.y + m_OrbitRadius * sinPhi;
				m_Position.z = m_OrbitCenter.z + m_OrbitRadius * cosTheta * cosPhi;
			}
		}

	}



	void EditorCamera::HandleKeyboardInput(float deltaTime)
	{
		float moveAmount = m_MoveSpeed * deltaTime;

		if (Input::KeyPressed(KeyCode(Key::KEY_W)))
			m_Position += glm::vec3(0.0f, 0.0f, -1.0f) * moveAmount;
		if (Input::KeyPressed(KeyCode(Key::KEY_S)))
			m_Position += glm::vec3(0.0f, 0.0f, 1.0f) * moveAmount;
		if (Input::KeyPressed(KeyCode(Key::KEY_A)))
			m_Position += glm::vec3(-1.0f, 0.0f, 0.0f) * moveAmount;
		if (Input::KeyPressed(KeyCode(Key::KEY_D)))
			m_Position += glm::vec3(1.0f, 0.0f, 0.0f) * moveAmount;

		if (Input::KeyPressed(KeyCode(Key::SPACE)))
		{
			m_CameraState = (m_CameraState == CameraState::FREE_CAMERA) ? CameraState::ORBIT_CAMERA : CameraState::FREE_CAMERA;
		}
	}

	void EditorCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		ZERO_ASSERT(ERROR, width > 0 && height > 0, "Viewport incorrecto para la camara");
		m_Aspect = (float)width / (float)height;
		CalculateProjection();
	}

	void EditorCamera::CalculateView()
	{
		if (m_CameraState == CameraState::FREE_CAMERA)
		{
			glm::mat4 transform = glm::mat4(1.0f);
			transform = glm::translate(transform, m_Position);
			transform = glm::rotate(transform, m_Rotation.x, glm::vec3(1, 0, 0));
			transform = glm::rotate(transform, m_Rotation.y, glm::vec3(0, 1, 0));
			transform = glm::rotate(transform, m_Rotation.z, glm::vec3(0, 0, 1));

			m_View = glm::inverse(transform);
		}
		else
		{
			glm::vec3 dir = glm::normalize(m_Position - m_OrbitCenter);
			glm::vec3 up = glm::vec3(0, 1, 0);
			m_View = glm::lookAt(m_Position, m_OrbitCenter, up);
		}
	}

	// ... (implementar los otros métodos como SetPerspectiveProjection, CalculateProjection, etc.)
}