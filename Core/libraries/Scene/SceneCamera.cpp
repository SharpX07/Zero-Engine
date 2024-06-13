#include "SceneCamera.h"
#include "Core/Assert.h"

namespace Zero
{
	void SceneCamera::SetPerspectiveProjection(float fovy, float zNear, float zFar)
	{
		m_PerspectiveFOV = fovy;
		m_PerspectiveNear = zNear;
		m_PerspectiveFar = zFar;
	}

	void SceneCamera::CalculateProjection()
	{
		m_Projection = glm::perspective(m_PerspectiveFOV, m_Aspect, m_PerspectiveNear, m_PerspectiveFar);
	}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		ZERO_ASSERT(ERROR, width > 0 && height > 0, "Viewport incorrecto para la camara");
		m_Aspect = (float)width / (float)height;
		CalculateProjection();
	}

	void SceneCamera::CalculateView(glm::mat4& transform)
	{
		m_View = glm::inverse(transform);
	}

	glm::mat4 SceneCamera::perspective(float fovy, float aspect, float zNear, float zFar)
	{
		return glm::mat4();
	}
	
}
