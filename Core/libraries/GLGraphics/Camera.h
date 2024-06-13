#pragma once
#include <glm/glm.hpp>
namespace Zero
{
	class Camera
	{
	public:

		Camera(const glm::mat4& m_Projection, const glm::mat4& m_View)
			: m_Projection(m_Projection), m_View(m_View) {}
		virtual glm::mat4 GetProjection() = 0;
		virtual glm::mat4 GetView() = 0;
	protected:
		glm::mat4 m_Projection;
		glm::mat4 m_View;	
	};
}