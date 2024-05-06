#pragma once

#include <glm.hpp>
#include <glad/glad.h>
#include <Debug/Logger.h>

#include <string>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>



namespace Zero
{
	class Shader {
	public:
		Shader(const char* vertexPath, const char* fragmentPath);
        ~Shader();
		void Use();
		void UnUse();
        void link();
        void unlink();

        inline void setBool(const std::string& name, bool value) const
        {
            glUniform1i(glGetUniformLocation(m_ShaderProgram, name.c_str()), (int)value);
        }
        // ------------------------------------------------------------------------
        inline void setInt(const std::string& name, int value) const
        {
            glUniform1i(glGetUniformLocation(m_ShaderProgram, name.c_str()), value);
        }
        // ------------------------------------------------------------------------
        inline void setFloat(const std::string& name, float value) const
        {
            glUniform1f(glGetUniformLocation(m_ShaderProgram, name.c_str()), value);
        }
        // ------------------------------------------------------------------------
        inline void setVec2(const std::string& name, const glm::vec2& value) const
        {
            glUniform2fv(glGetUniformLocation(m_ShaderProgram, name.c_str()), 1, &value[0]);
        }
        inline void setVec2(const std::string& name, float x, float y) const
        {
            glUniform2f(glGetUniformLocation(m_ShaderProgram, name.c_str()), x, y);
        }
        // ------------------------------------------------------------------------
        inline void setVec3(const std::string& name, const glm::vec3& value) const
        {
            glUniform3fv(glGetUniformLocation(m_ShaderProgram, name.c_str()), 1, &value[0]);
        }
        inline void setVec3(const std::string& name, float x, float y, float z) const
        {
            glUniform3f(glGetUniformLocation(m_ShaderProgram, name.c_str()), x, y, z);
        }
        // ------------------------------------------------------------------------
        inline void setVec4(const std::string& name, const glm::vec4& value) const
        {
            glUniform4fv(glGetUniformLocation(m_ShaderProgram, name.c_str()), 1, &value[0]);
        }
        inline void setVec4(const std::string& name, float x, float y, float z, float w)
        {
            glUniform4f(glGetUniformLocation(m_ShaderProgram, name.c_str()), x, y, z, w);
        }
        // ------------------------------------------------------------------------
        inline void setMat2(const std::string& name, const glm::mat2& mat) const
        {
            glUniformMatrix2fv(glGetUniformLocation(m_ShaderProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }
        // ------------------------------------------------------------------------
        inline void setMat3(const std::string& name, const glm::mat3& mat) const
        {
            glUniformMatrix3fv(glGetUniformLocation(m_ShaderProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }
        // ------------------------------------------------------------------------
        inline void setMat4(const std::string& name, const glm::mat4& mat) const
        {
            glUniformMatrix4fv(glGetUniformLocation(m_ShaderProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }

        inline void setTexture3D(const std::string& name, GLuint textureID) const
        {
            // Asociar el ID de la textura 3D al nombre en el shader
            glUniform1i(glGetUniformLocation(m_ShaderProgram, name.c_str()), 0);

            // Activar la unidad de textura 0
            glActiveTexture(GL_TEXTURE0);

            // Vincular la textura 3D al shader
            glBindTexture(GL_TEXTURE_3D, textureID);
        }  

		void SetVertexAttribute(const char* name, int size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);
		const char* GetLastError();

	private:
		void checkCompileErrors(GLuint shader, std::string type);
		GLuint m_VertexShader;
		GLuint m_FragmentShader;
    public:
		GLuint m_ShaderProgram;
	};
}
