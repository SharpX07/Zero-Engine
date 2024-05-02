#pragma once
#include <glad/glad.h>

namespace zeroengine
{
	class VBO
	{
	public:
		VBO(const void* _data, const size_t _size);
		~VBO();
	public:
		void Bind() const;
		void UnBind();
		GLuint Id_;
	};
}
