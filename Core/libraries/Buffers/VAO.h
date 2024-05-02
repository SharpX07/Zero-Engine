#pragma once
#include <glad/glad.h>
#include <Buffers/VBO.h>
#include <Buffers/AttributeLayout.h>

namespace zeroengine
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