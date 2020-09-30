#pragma once
#ifndef TEXT_H
#define TEXT_H

#include <string>

#include "../graphics/Colour.h"
#include "../graphics/renderer/Renderer.h"
#include "../graphics/texture/Texture.h"
#include "font/Font.h"

namespace stardust
{
	namespace text
	{
		[[nodiscard]] extern Texture RenderGlyph(const Renderer& renderer, const Font& font, const char glyph, const Colour& colour);
		[[nodiscard]] extern Texture RenderGlyph(const Renderer& renderer, const Font& font, const char16_t glyph, const Colour& colour);
		[[nodiscard]] extern Texture RenderText(const Renderer& renderer, const Font& font, const std::string& text, const Colour& colour);
		[[nodiscard]] extern Texture RenderText(const Renderer& renderer, const Font& font, const std::u16string& text, const Colour& colour);

		[[nodiscard]] extern Texture RenderTextWrapped(const Renderer& renderer, const Font& font, const std::string& text, const Colour& colour, const unsigned int wrapLength);
		[[nodiscard]] extern Texture RenderTextWrapped(const Renderer& renderer, const Font& font, const std::u16string& text, const Colour& colour, const unsigned int wrapLength);

		[[nodiscard]] extern Texture RenderGlyphQuick(const Renderer& renderer, const Font& font, const char glyph, const Colour& colour);
		[[nodiscard]] extern Texture RenderGlyphQuick(const Renderer& renderer, const Font& font, const char16_t glyph, const Colour& colour);
		[[nodiscard]] extern Texture RenderTextQuick(const Renderer& renderer, const Font& font, const std::string& text, const Colour& colour);
		[[nodiscard]] extern Texture RenderTextQuick(const Renderer& renderer, const Font& font, const std::u16string& text, const Colour& colour);

		[[nodiscard]] extern Texture RenderShadedGlyph(const Renderer& renderer, const Font& font, const char glyph, const Colour& colour, const Colour& backgroundColour);
		[[nodiscard]] extern Texture RenderShadedGlyph(const Renderer& renderer, const Font& font, const char16_t glyph, const Colour& colour, const Colour& backgroundColour);
		[[nodiscard]] extern Texture RenderShadedText(const Renderer& renderer, const Font& font, const std::string& text, const Colour& colour, const Colour& backgroundColour);
		[[nodiscard]] extern Texture RenderShadedText(const Renderer& renderer, const Font& font, const std::u16string& text, const Colour& colour, const Colour& backgroundColour);
	}
}

#endif