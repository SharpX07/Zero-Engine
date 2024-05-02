#include "EBO.h"

zeroengine::EBO::EBO(const void* _indices, size_t size)
{
	glGenBuffers(1, &m_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(unsigned int), _indices,
		GL_STATIC_DRAW);
}

zeroengine::EBO::~EBO()
{
	glDeleteBuffers(1, &m_id);
}

void zeroengine::EBO::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
}

void zeroengine::EBO::UnBind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
