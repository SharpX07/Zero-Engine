#pragma once
#include <GLGraphics/Camera.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Zero
{
	class SceneCamera : public Camera
	{
	public:
		SceneCamera(const glm::mat4& projection, const glm::mat4& view)
			: Camera(projection, view), m_Projection(projection), m_View(view) {}

		void SetPerspectiveProjection(float fovy, float zNear, float zFar);
		//void SetOrthographicProjection();
		void CalculateProjection();
		void CalculateView(glm::mat4& transform);
		void SetViewportSize(uint32_t width, uint32_t height);

		glm::mat4 GetProjection() override { return m_Projection; }
		void SetProjection(const glm::mat4& projection) { m_Projection = projection; }
		glm::mat4 GetView() override { return m_View; }
		void SetView(const glm::mat4& view) { m_View = view; }

		
		glm::mat4 perspective(float fovy, float aspect, float zNear,
			float zFar);

	private:
		glm::mat4 m_Projection;
		glm::mat4 m_View;
		float m_Aspect;
		
		float m_PerspectiveFOV = glm::radians(45.0f);
		float m_PerspectiveNear = 0.01f, m_PerspectiveFar = 1000.0f;

	};
}
