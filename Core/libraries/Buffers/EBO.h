#pragma once
#include <glad/glad.h>

namespace zeroengine
{
	class EBO
	{
	public:
		EBO(const void* _indices, size_t size);
		~EBO();
		void Bind() const;
		void UnBind();

	private:
		GLuint m_id;
	};
}

