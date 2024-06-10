#include "Shader.h"
#include <core/Logger.h>
namespace Zero {
	Shader::Shader(const char* vertexPath, const char* fragmentPath) {
        // 1. retrieve the vertex/fragment source code from filePath
        std::string vertexCode;
        std::string fragmentCode;
        std::string geometryCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        std::ifstream gShaderFile;
        // ensure ifstream objects can throw exceptions:
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            // open files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            // read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // close file handlers
            vShaderFile.close();
            fShaderFile.close();
            // convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
            // if geometry shader path is present, also load a geometry shader
           
        }
        catch (std::ifstream::failure& e)
        {
            ZERO_CORE_LOG_ERROR("SHADER::FILE_NOT_SUCCESSFULLY_READ:{0}", e.what());
        }
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();
        // 2. compile shaders
        unsigned int vertex, fragment;
        // vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");
        // fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");
        // if geometry shader is given, compile geometry shader
        // unsigned int geometry;
        
        // shader Program
        m_ShaderProgram = glCreateProgram();
        glAttachShader(m_ShaderProgram, vertex);
        glAttachShader(m_ShaderProgram, fragment);
        glLinkProgram(m_ShaderProgram);
        checkCompileErrors(m_ShaderProgram, "PROGRAM");
        // delete the shaders as they're linked into our program now and no longer necessary
        glDeleteShader(vertex);
        glDeleteShader(fragment);

	}

    Shader::~Shader()
    {
        glDeleteProgram(m_ShaderProgram);
    }

	void Shader::Use() {
		glUseProgram(m_ShaderProgram);
	}

	void Shader::UnUse() {
		glUseProgram(0);
	}

	void Shader::SetVertexAttribute(const char* name, int size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer) {
		GLint location = glGetAttribLocation(m_ShaderProgram, name);
		glVertexAttribPointer(location, size, type, normalized, stride, pointer);
		glEnableVertexAttribArray(location);
	}

	const char* Shader::GetLastError() {
		return nullptr;
	}

	void Shader::checkCompileErrors(GLuint shader, std::string type)
	{
		Log logger;
		GLint success;
		GLchar infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                ZERO_CORE_LOG_ERROR("SHADER_COMPILATION_ERROR of type: {0} \n{1}", type, infoLog);
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                ZERO_CORE_LOG_ERROR("SHADER_COMPILATION_ERROR of type: {0} \n{1}", type, infoLog);
			}
		}

	}
}