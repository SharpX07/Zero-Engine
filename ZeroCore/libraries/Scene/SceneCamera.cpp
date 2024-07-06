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
		// Extraer la posici�n de la c�mara de la matriz de transformaci�n
		glm::vec3 eye = glm::vec3(transform[3]);

		// Definir el punto al que la c�mara est� mirando (por ejemplo, hacia adelante desde la posici�n de la c�mara)
		glm::vec3 forward = glm::normalize(glm::vec3(transform[2]));  // La direcci�n hacia adelante es la tercera columna
		glm::vec3 center = eye - forward; // La c�mara mira en la direcci�n opuesta al "forward"

		// Definir el vector "arriba" del mundo (esto depende de tu configuraci�n, generalmente es el eje Y)
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
		// Calcular la matriz de vista usando glm::lookAt
		m_View = glm::lookAt(eye, glm::vec3(0,0,0), up);
	}

	glm::mat4 SceneCamera::perspective(float fovy, float aspect, float zNear, float zFar)
	{
		return glm::mat4();
	}
	
}
