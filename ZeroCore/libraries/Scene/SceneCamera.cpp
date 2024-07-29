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
		//ZERO_ASSERT(ERROR, width > 0 && height > 0, "Viewport incorrecto para la camara");
		if (!(width > 0 && height > 0))
			return;
		m_Aspect = (float)width / (float)height;
		CalculateProjection();
	}

	void SceneCamera::CalculateView(glm::mat4& transform)
	{
		// Extraer la posición de la cámara de la matriz de transformación
		glm::vec3 eye = glm::vec3(transform[3]);

		// Extraer los vectores de dirección de la matriz de transformación
		glm::vec3 right = glm::normalize(glm::vec3(transform[0])); // Primera columna
		glm::vec3 up = glm::normalize(glm::vec3(transform[1]));    // Segunda columna
		glm::vec3 forward = glm::normalize(glm::vec3(transform[2])); // Tercera columna

		// Calcular la matriz de vista usando glm::lookAt
		glm::vec3 center = eye + forward;
		m_View = glm::lookAt(eye, center, up);
	}


	glm::mat4 SceneCamera::perspective(float fovy, float aspect, float zNear, float zFar)
	{
		return glm::mat4();
	}
	
}
