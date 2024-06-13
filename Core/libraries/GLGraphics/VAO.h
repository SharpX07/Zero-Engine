#pragma once
#include <glad/glad.h>
#include <GLGraphics/VBO.h>
#include <GLGraphics/AttributeLayout.h>

namespace Zero
{
	class VAO
	{
	public:
		VAO();
		~VAO();
		void Bind() const;
		void UnBind() const;
		void AddBuffer(const VBO &buffer, const VertexAttributeLayout &layout);
	private:
		void InitBuffer();
		GLuint m_id;
	};
}