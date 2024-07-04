#pragma once
#include <glad/glad.h>

namespace Zero
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

