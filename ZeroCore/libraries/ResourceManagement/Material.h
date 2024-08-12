#pragma once

#include <array>
#include <GLGraphics/Texture.h>
#include <Core/Aliases.h>
#include <Core/UUID.h>

namespace Zero
{
	enum MeshTextureTypes {
		DIFFUSE,
		SPECULAR,
		NORMAL,
		ROUGHNESS,
		METALNESS,
		EMISSIVE
	};
	struct MeshTexture {
		Ref<Zero::GLTexture> GlTexture;
		MeshTextureTypes Type;
		std::string Path;
		UUID Identifier;
	};
	struct MaterialProperties {
		bool hasAlbedoTexture = false;
		glm::vec3 Diffuse = glm::vec3(0.0f);
		glm::vec3 Specular = glm::vec3(0.0f);
		glm::vec3 Ambient = glm::vec3(0.0f);
		float Shininess = 0.0;
		float Metallic = 0.0f;
		float Roughness = 0.5f;
		glm::vec3 Emissive = glm::vec3(0.0f);
		float Opacity = 1.0f;
	};
	class Material
	{
	public:

		Material() = default;
		~Material() = default;
		inline const std::array<MeshTexture,5>& getTextures() const {
			return m_textures;
		}
		void addTexture(const MeshTexture& texture) {
			m_textures.at(m_NumTextures++) = texture;
		}
		std::array<MeshTexture, 5> m_textures;
		MaterialProperties& GetProperties() { return m_Properties; }
		inline unsigned int GetNumTextures() const { return m_NumTextures; }
	private:
		unsigned int m_NumTextures=0;
		MaterialProperties m_Properties;
	};
}
