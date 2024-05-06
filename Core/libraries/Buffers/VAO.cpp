#include <Buffers/VAO.h>

namespace Zero
{
	VAO::VAO() : m_id(0)
	{
		InitBuffer();
	}

	VAO::~VAO()
	{
		glDeleteVertexArrays(1, &m_id);
	}

	void VAO::InitBuffer()
	{
		glGenVertexArrays(1, &m_id);
	}

	void VAO::Bind() const
	{
		glBindVertexArray(m_id);
	}

	void VAO::UnBind() const
	{
		glBindVertexArray(0);
	}
	void VAO::AddBuffer(const VBO &buffer, const VertexAttributeLayout &layout)
	{
		buffer.Bind();
		const auto& elements = layout.GetAttributeElements();
		unsigned int offset=0;
		for (unsigned int i = 0; i < elements.size(); i++)
		{
			const auto& element = elements.at(i);
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, element.count, element.type, element.normalize, layout.GetStride(), (void*)offset);
			offset+= element.count*element.GetSizeOfGlType(element.type);
		}
	}
}