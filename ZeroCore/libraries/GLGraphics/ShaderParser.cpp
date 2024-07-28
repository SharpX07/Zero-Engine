#include "ShaderParser.h"
#include <Core/Logger.h>

namespace Zero
{
	// Implementación de los métodos
	Ref<Shader> ShaderParser::GenerateShader(const std::string& filePath)
	{
		if (!ProcessShaderFile(filePath)) return nullptr;
		Ref<Shader> newShader = CreateRef<Shader>(m_VertexSourceCode.c_str(), m_FragmentSourceCode.c_str(), ShaderSourceType::Source);
		newShader->SetName(filePath);
		m_VertexSourceCode.clear();
		m_FragmentSourceCode.clear();
		return newShader;
	}

	bool ShaderParser::ProcessShaderFile(const std::string& filePath)
	{
		std::ifstream file(filePath);
		if (!file.is_open())
		{
			ZERO_CORE_LOG_WARN("Failed to open shader file: {0}", filePath);
			return false;
			//throw std::runtime_error("Failed to open shader file: " + filePath);
		}

		std::string line;
		std::string currentShaderType;

		while (std::getline(file, line))
		{
			ProcessLine(line, currentShaderType);
		}
		return true;
	}

	void ShaderParser::ProcessLine(const std::string& line, std::string& currentShaderType)
	{
		if (line.find("#type") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				currentShaderType = "vertex";
			else if (line.find("fragment") != std::string::npos)
				currentShaderType = "fragment";
		}
		else if (!currentShaderType.empty())
		{
			if (currentShaderType == "vertex")
				m_VertexSourceCode += line + "\n";
			if (currentShaderType == "fragment")
				m_FragmentSourceCode += line + "\n";
		}
	}
}