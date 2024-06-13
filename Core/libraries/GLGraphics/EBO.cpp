#include "EBO.h"

Zero::EBO::EBO(const void* _indices, size_t size)
{
	glGenBuffers(1, &m_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, _indices,
		GL_STATIC_DRAW);
}

Zero::EBO::~EBO()
{
	glDeleteBuffers(1, &m_id);
}

void Zero::EBO::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
}

void Zero::EBO::UnBind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
