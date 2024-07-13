#pragma once
#include <string>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <GLGraphics/Shader.h>
#include <Core/Aliases.h>
namespace Zero
{
    class ShaderParser {
    public:
        ShaderParser() = default;
        ~ShaderParser() = default;
        void ProcessShaderFile(const std::string& filePath);
        Ref<Shader> GenerateShader(const std::string& filePath);
        // Verifica si un tipo de shader espec�fico est� presente
        bool HasShaderType(const std::string& shaderType) const;

    private:
        // M�todo privado para procesar cada l�nea del archivo
        void ProcessLine(const std::string& line, std::string& currentShaderType);
        std::string m_VertexSourceCode;
        std::string m_FragmentSourceCode;
    };

    
}
