#pragma once

#include <array>
#include <GLGraphics/Texture.h>
#include <Core/Aliases.h>
#include <Core/UUID.h>
#include <ResourceManagement/Resource.h>

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
		UUID Identifier;
	};
	struct MaterialProperties {
		glm::vec3 BaseColor = { 0.0f,0.0f ,0.0f };
		float MetallicFactor = 0.0f;
		float RoughnessFactor = 0.0f;
	};
	class Material : public Resource
	{
	public:

		Material() = default;
		~Material() = default;
		inline const std::array<MeshTexture, 5>& getTextures() const {
			return m_textures;
		}
		void addTexture(const MeshTexture& texture) {
			m_textures.at(m_NumTextures++) = texture;
		}
		std::array<MeshTexture, 5> m_textures;
		MaterialProperties& GetProperties() { return m_Properties; }
		inline unsigned int GetNumTextures() const { return m_NumTextures; }
	private:
		unsigned int m_NumTextures = 0;
		MaterialProperties m_Properties;
	};
}
