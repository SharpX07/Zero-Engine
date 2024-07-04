#pragma once
#include <glad/glad.h>
#include <vector>
namespace Zero
{
	struct AttributeElement
	{
		int count;
		unsigned int type;
		bool normalize;
		static unsigned int GetSizeOfGlType(unsigned int type)
		{
			switch (type)
			{
			case GL_FLOAT:
				return sizeof(GLfloat);
			case GL_INT:
				return sizeof(GLint);
			case GL_UNSIGNED_INT:
				return sizeof(GLuint);
			case GL_BYTE:
				return sizeof(GLbyte);
			case GL_UNSIGNED_BYTE:
				return sizeof(GLubyte);
			}
			return 0;
		}
	};

	class VertexAttributeLayout
	{
	public:
		VertexAttributeLayout() :m_stride(0) {};

		void addAttribute(unsigned int type, int count)
		{
			m_elements.push_back({ count,type,false });
			m_stride += m_elements.at(m_elements.size()-1).GetSizeOfGlType(type) * count;
		}
		inline const std::vector<AttributeElement> GetAttributeElements() const& { return m_elements; }
		inline unsigned int GetStride() const { return m_stride; }
	private:
		std::vector<AttributeElement> m_elements;
		unsigned int m_stride;
	};

}



