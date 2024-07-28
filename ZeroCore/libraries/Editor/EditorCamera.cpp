#include "EditorCamera.h"
#include <Core/Assert.h>
#include <Events/MouseEvents.h>
namespace Zero
{
	void EditorCamera::Update(float deltaTime)
	{
		HandleMouseInput(deltaTime);
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
	void EditorCamera::HandleMouseInput(float deltaTime)
	{
		glm::vec2 currentMousePos;
		glm::vec2 delta;
		currentMousePos = Input::GetMousePosition();
		delta = currentMousePos - m_LastMousePosition;
		m_LastMousePosition = currentMousePos;

		if (m_CameraState == CameraState::ORBIT_CAMERA)
		{
			if (Input::MouseScrolled())
			{
				auto scrollValue = Input::GetMouseScrollDelta();
				m_OrbitRadius -= scrollValue * (m_OrbitRadius / 10);
			}
			if (Input::MousePressed(Zero::MouseCode(Zero::MouseButton::MIDDLE)))
			{
				if (Input::KeyPressed(Key::LEFT_SHIFT))
				{
					m_OrbitCenter += (m_Up * delta.y - m_Right * delta.x) * deltaTime * (m_OrbitRadius / 100);
				}
				else
				{
					m_Yaw -= glm::radians(delta.x * m_MouseSensitivity);
					m_Pitch += glm::radians(delta.y * m_MouseSensitivity);
				}
			}

			if (m_Yaw > glm::pi<float>()) m_Yaw = -glm::pi<float>();
			if (m_Yaw < -glm::pi<float>()) m_Yaw = glm::pi<float>();

			float sinTheta = glm::sin(m_Yaw);
			float cosTheta = glm::cos(m_Yaw);
			float sinPhi = glm::sin(m_Pitch);
			float cosPhi = glm::cos(m_Pitch);

			m_Position = m_OrbitCenter + m_OrbitRadius * glm::vec3(sinTheta * cosPhi, sinPhi, cosTheta * cosPhi);
			m_Front = glm::normalize(m_OrbitCenter - m_Position);
			m_Right = glm::normalize(glm::cross(m_Front, glm::vec3(0.0f, 1.0f, 0.0f)));
			m_Up = glm::normalize(glm::cross(m_Right, m_Front));
		}
		else
		{
			if (Input::MousePressed(Zero::MouseCode(Zero::MouseButton::RIGHT)))
			{
				m_Yaw += glm::radians(delta.x * m_MouseSensitivity);
				m_Pitch -= glm::radians(delta.y * m_MouseSensitivity);
				m_Pitch = glm::clamp(m_Pitch, -glm::half_pi<float>() + 0.1f, glm::half_pi<float>() - 0.1f);

				glm::vec3 front;
				front.x = cos(m_Yaw) * cos(m_Pitch);
				front.y = sin(m_Pitch);
				front.z = sin(m_Yaw) * cos(m_Pitch);
				m_Front = glm::normalize(front);
				m_Right = glm::normalize(glm::cross(m_Front, glm::vec3(0.0f, 1.0f, 0.0f)));
				m_Up = glm::normalize(glm::cross(m_Right, m_Front));
			}
		}
	}


	void EditorCamera::HandleKeyboardInput(float deltaTime)
	{
		float moveAmount = m_MoveSpeed * deltaTime;
		if (m_CameraState == CameraState::FREE_CAMERA)
		{
			if (Input::KeyPressed(Key::KEY_W))
				m_Position += m_Front * moveAmount;
			if (Input::KeyPressed(Key::KEY_S))
				m_Position -= m_Front * moveAmount;
			if (Input::KeyPressed(Key::KEY_A))
				m_Position -= m_Right * moveAmount;
			if (Input::KeyPressed(Key::KEY_D))
				m_Position += m_Right * moveAmount;
		}
		if (Input::KeyDown(Key::SPACE))
		{
			ChangeCameraState(m_CameraState == CameraState::ORBIT_CAMERA ? CameraState::FREE_CAMERA : CameraState::ORBIT_CAMERA);
		}
	}

	void EditorCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		//ZERO_ASSERT(ERROR, width > 0 && height > 0, "Viewport incorrecto para la camara");
		if (!(width > 0 && height > 0))
			return;
		m_Aspect = (float)width / (float)height;
		CalculateProjection();
	}

	void EditorCamera::CalculateView()
	{
		if (m_CameraState == CameraState::ORBIT_CAMERA)
			m_View = glm::lookAt(m_Position, m_OrbitCenter, m_Up);
		else
			m_View = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
	}

	void EditorCamera::ChangeCameraState(CameraState newState)
	{
		if (m_CameraState == newState) return;

		if (m_CameraState == CameraState::ORBIT_CAMERA && newState == CameraState::FREE_CAMERA)
		{
			// Transición de ORBIT_CAMERA a FREE_CAMERA
			m_Front = glm::normalize(m_OrbitCenter - m_Position);
			m_Right = glm::normalize(glm::cross(m_Front, glm::vec3(0.0f, 1.0f, 0.0f)));
			m_Up = glm::normalize(glm::cross(m_Right, m_Front));

			// Calcular yaw y pitch basados en el vector Front
			m_Yaw = atan2(m_Front.z, m_Front.x);
			m_Pitch = asin(m_Front.y);
		}
		else if (m_CameraState == CameraState::FREE_CAMERA && newState == CameraState::ORBIT_CAMERA)
		{
			// Transición de FREE_CAMERA a ORBIT_CAMERA
			m_OrbitRadius = glm::length(m_Position - m_OrbitCenter);
			glm::vec3 normalizedPosition = (m_Position - m_OrbitCenter) / m_OrbitRadius;
			m_Pitch = glm::asin(normalizedPosition.y);
			m_Yaw = glm::atan(normalizedPosition.x, normalizedPosition.z);
		}

		m_CameraState = newState;
	}
}