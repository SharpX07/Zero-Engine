#pragma once

#include <array>
#include <GL_graphics/Texture.h>

namespace Zero
{
	// Gltexture*, MeshTexturetype, Path
	enum MeshTextureTypes {
		DIFFUSE,
		SPECULAR,
		NORMAL,
		ROUGHNESS,
		METALNESS,
		EMISSIVE
	};
	struct MeshTexture {
		std::shared_ptr<Zero::GLTexture> GlTexture;
		MeshTextureTypes Type;
		std::string Path;
	};

	class Material
	{
	public:

		//Material();
		//~Material();
		inline const std::array<MeshTexture,5>& getTextures() const {
			return m_textures;
		}
		void addTexture(const MeshTexture& texture) {
			m_textures.at(m_NumTextures++) = texture;
		}
		std::array<MeshTexture, 5> m_textures;
		inline unsigned int GetNumTextures() const { return m_NumTextures; }
	private:
		unsigned int m_NumTextures=0;
	};
}
