#include "Shader.h"
#include <Core/Logger.h>
namespace Zero {
	Shader::Shader(const char* vertexSource, const char* fragmentSource, ShaderSourceType type) {
		// 1. retrieve the vertex/fragment source code from fileSource
		if (type == ShaderSourceType::Path)
			CreateShaderByPaths(vertexSource, fragmentSource);
		else
			CreateShaderByCode(vertexSource, fragmentSource);
		
			

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
	GLuint Shader::CreateVertexShader(const char* vShaderCode)
	{
		GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		checkCompileErrors(vertex, "VERTEX");
		return vertex;
	}
	GLuint Shader::CreateFragmentShader(const char* fShaderCode)
	{
		GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		checkCompileErrors(fragment, "FRAGMENT");
		return fragment;
	}
	void Shader::CreateShaderProgram(GLuint vertex, GLuint fragment)
	{
		m_ShaderProgram = glCreateProgram();
		glAttachShader(m_ShaderProgram, vertex);
		glAttachShader(m_ShaderProgram, fragment);
		glLinkProgram(m_ShaderProgram);
		checkCompileErrors(m_ShaderProgram, "PROGRAM");
	}
	void Shader::CreateShaderByPaths(const char* vertexPath, const char* fragmentPath)
	{
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		std::ifstream gShaderFile;
		// ensure ifstream objects can throw exceptions:
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
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
		vertex = CreateVertexShader(vShaderCode);
		fragment = CreateFragmentShader(fShaderCode);
		CreateShaderProgram(vertex, fragment);

		// delete the shaders as they're linked into our program now and no longer necessary
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}
	void Shader::CreateShaderByCode(const char* vertexCode, const char* fragmentCode)
	{
		unsigned int vertex, fragment;
		vertex = CreateVertexShader(vertexCode);
		fragment = CreateFragmentShader(fragmentCode);
		CreateShaderProgram(vertex, fragment);

		// delete the shaders as they're linked into our program now and no longer necessary
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}
}