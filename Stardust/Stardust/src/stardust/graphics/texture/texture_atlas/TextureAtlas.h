#pragma once
#ifndef TEXTURE_ATLAS_H
#define TEXTURE_ATLAS_H

#include "../../../utility/interfaces/INoncopyable.h"

#include <cstddef>
#include <string>
#include <string_view>
#include <unordered_map>

#include "../../../rect/Rect.h"
#include "../../renderer/Renderer.h"
#include "../Texture.h"

namespace stardust
{
	class TextureAtlas
		: private INoncopyable
	{
	private:
		Texture m_texture;

		std::unordered_map<std::string, rect::Rect> m_subtextures{ };

	public:
		TextureAtlas() = default;
		TextureAtlas(const Renderer& renderer, const std::string_view& filepath);

		TextureAtlas(TextureAtlas&& other) noexcept;
		TextureAtlas& operator =(TextureAtlas&& other) noexcept;

		~TextureAtlas() noexcept;

		void Initialise(const Renderer& renderer, const std::string_view& filepath);
		void Destroy() noexcept;

		inline bool IsTextureValid() const noexcept { return m_texture.IsValid(); }

		inline const rect::Rect& GetSubtexture(const std::string& name) const { return m_subtextures.at(name); }
		inline const rect::Rect& operator [](const std::string& name) const { return GetSubtexture(name); }

		inline Texture& GetTexture() noexcept { return m_texture; }
		inline const Texture& GetTexture() const noexcept { return m_texture; }
		inline const decltype(m_subtextures)& GetSubtextures() const noexcept { return m_subtextures; }

		inline const std::size_t GetSubtextureCount() const noexcept { return m_subtextures.size(); }
	};
}

#endif