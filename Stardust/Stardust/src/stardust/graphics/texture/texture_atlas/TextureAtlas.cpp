#include "TextureAtlas.h"

#include <utility>
#include <vector>

#include <nlohmann/json.hpp>

#include "../../../vfs/VFS.h"

namespace stardust
{
	TextureAtlas::TextureAtlas(const Renderer& renderer, const std::string_view& filepath)
	{
		Initialise(renderer, filepath);
	}

	TextureAtlas::TextureAtlas(TextureAtlas&& other) noexcept
		: m_texture(std::move(other.m_texture)), m_subtextures(other.m_subtextures)
	{
		other.m_texture = Texture();
		other.m_subtextures = { };
	}

	TextureAtlas& TextureAtlas::operator =(TextureAtlas&& other) noexcept
	{
		m_texture = std::exchange(other.m_texture, Texture());
		m_subtextures = std::exchange(other.m_subtextures, { });

		return *this;
	}

	TextureAtlas::~TextureAtlas() noexcept
	{
		Destroy();
	}

	void TextureAtlas::Initialise(const Renderer& renderer, const std::string_view& filepath)
	{
		const std::vector<std::byte> textureAtlasData = vfs::ReadFileData(filepath);

		if (textureAtlasData.empty())
		{
			return;
		}

		const nlohmann::json textureAtlasJSON = nlohmann::json::parse(
			reinterpret_cast<const unsigned char*>(textureAtlasData.data()),
			reinterpret_cast<const unsigned char*>(textureAtlasData.data()) + textureAtlasData.size(),
			nullptr,
			false
		);

		if (textureAtlasJSON.is_discarded())
		{
			return;
		}
		
		const std::string textureFilename = vfs::GetParentFilepath(filepath) + "/" + std::string(textureAtlasJSON["texture_file"]);
		m_texture = Texture(renderer, textureFilename);

		for (const auto& subtexture : textureAtlasJSON["subtextures"])
		{
			const rect::Rect subtextureRect = rect::Create(
				subtexture["rect"]["x"],
				subtexture["rect"]["y"],
				subtexture["rect"]["width"],
				subtexture["rect"]["height"]
			);

			m_subtextures[subtexture["name"]] = subtextureRect;
		}
	}

	void TextureAtlas::Destroy() noexcept
	{
		if (m_texture.IsValid())
		{
			m_texture.Destroy();
			m_subtextures.clear();
		}
	}
}