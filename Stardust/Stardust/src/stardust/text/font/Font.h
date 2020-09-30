#pragma once
#ifndef FONT_H
#define FONT_H

#include "../../utility/interfaces/INoncopyable.h"

#include <cstddef>
#include <memory>
#include <optional>
#include <string_view>
#include <vector>

#include <glm/glm.hpp>
#include <SDL2/SDL_ttf.h>

namespace stardust
{
	class Font
		: private INoncopyable
	{
	public:
		enum class Style
			: decltype(TTF_STYLE_NORMAL)
		{
			Normal = TTF_STYLE_NORMAL,
			Bold = TTF_STYLE_BOLD,
			Italic = TTF_STYLE_ITALIC,
			Underline = TTF_STYLE_UNDERLINE,
			Strikethrough = TTF_STYLE_STRIKETHROUGH,
		};

		enum class Hinting
			: decltype(TTF_HINTING_NORMAL)
		{
			Normal = TTF_HINTING_NORMAL,
			Light = TTF_HINTING_LIGHT,
			Monochrome = TTF_HINTING_MONO,
			None = TTF_HINTING_NONE,
		};

		struct GlyphMetrics
		{
			glm::ivec2 minOffset;
			glm::ivec2 maxOffset;

			int advance;
		};

	private:
		struct FontDestroyer
		{
			void operator ()(TTF_Font* const font) const noexcept;
		};

		std::unique_ptr<TTF_Font, FontDestroyer> m_handle = nullptr;
		unsigned int m_pointSize = 0u;

	public:
		Font() = default;
		Font(const std::string_view& fontFilepath, const unsigned int pointSize);

		Font(Font&& other) noexcept;
		Font& operator =(Font&& other) noexcept;

		~Font() noexcept;

		void Initialise(const std::string_view& fontFilepath, const unsigned int pointSize);
		void Destroy() noexcept;

		inline bool IsValid() const noexcept { return m_handle != nullptr; }

		[[nodiscard]] std::vector<Style> GetStyles() const;
		void SetStyles(const std::vector<Style>& styles) const;
		void ClearStyles() const;

		[[nodiscard]] unsigned int GetOutlineThickness() const;
		void SetOutlineThickness(const unsigned int outlineThickness) const;
		void RemoveOutline() const;

		[[nodiscard]] Hinting GetHinting() const;
		void SetHinting(const Hinting hinting) const;

		[[nodiscard]] unsigned int GetKerning() const;
		void SetKerning(const unsigned int kerning) const;
		[[nodiscard]] unsigned int GetKerningBetweenGlyphs(const char16_t leftGlyph, const char16_t rightGlyph) const;

		[[nodiscard]] unsigned int GetMaximumHeight() const;
		[[nodiscard]] int GetFontAscent() const;
		[[nodiscard]] int GetFontDescent() const;
		[[nodiscard]] unsigned int GetLineSkip() const;

		[[nodiscard]] bool IsFixedWidth() const;

		[[nodiscard]] std::optional<std::size_t> GetGlyphIndex(const char16_t glyph) const;
		[[nodiscard]] bool DoesGlyphExist(const char16_t glyph) const;
		[[nodiscard]] GlyphMetrics GetGlyphMetrics(const char16_t glyph) const;

		[[nodiscard]] glm::uvec2 GetTextSize(const std::string& text) const;
		[[nodiscard]] glm::uvec2 GetTextSize(const std::u16string& text) const;

		[[nodiscard]] inline TTF_Font* const GetRawHandle() const noexcept { return m_handle.get(); }
	};
}

#endif