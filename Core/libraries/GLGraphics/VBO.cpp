#include <GLGraphics/VBO.h>

namespace Zero
{
	VBO::VBO(const void* _data, const size_t _size) : Id_(0)
	{
		glGenBuffers(1, &Id_);
		glBindBuffer(GL_ARRAY_BUFFER, Id_);
		glBufferData(GL_ARRAY_BUFFER, _size, _data, GL_STATIC_DRAW);
	}

	VBO::~VBO()
	{
		glDeleteBuffers(1, &Id_);
	}

	void VBO::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, Id_);
	}

	void VBO::UnBind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}
}
