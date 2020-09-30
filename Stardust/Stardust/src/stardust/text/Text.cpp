#include "Text.h"

#include <cstdint>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

namespace stardust
{
	namespace text
	{
		[[nodiscard]] Texture RenderGlyph(const Renderer& renderer, const Font& font, const char glyph, const Colour& colour)
		{
			SDL_Surface* renderedTextSurface = TTF_RenderGlyph_Blended(font.GetRawHandle(), glyph, colour);

			if (renderedTextSurface == nullptr)
			{
				return Texture();
			}

			Texture textTexture = Texture(renderer, renderedTextSurface);
			SDL_FreeSurface(renderedTextSurface);
			renderedTextSurface = nullptr;

			return textTexture;
		}

		[[nodiscard]] Texture RenderGlyph(const Renderer& renderer, const Font& font, const char16_t glyph, const Colour& colour)
		{
			SDL_Surface* renderedTextSurface = TTF_RenderGlyph_Blended(font.GetRawHandle(), static_cast<std::uint16_t>(glyph), colour);

			if (renderedTextSurface == nullptr)
			{
				return Texture();
			}

			Texture textTexture = Texture(renderer, renderedTextSurface);
			SDL_FreeSurface(renderedTextSurface);
			renderedTextSurface = nullptr;

			return textTexture;
		}

		[[nodiscard]] Texture RenderText(const Renderer& renderer, const Font& font, const std::string& text, const Colour& colour)
		{
			SDL_Surface* renderedTextSurface = TTF_RenderText_Blended(font.GetRawHandle(), text.c_str(), colour);

			if (renderedTextSurface == nullptr)
			{
				return Texture();
			}

			Texture textTexture = Texture(renderer, renderedTextSurface);
			SDL_FreeSurface(renderedTextSurface);
			renderedTextSurface = nullptr;

			return textTexture;
		}

		[[nodiscard]] Texture RenderText(const Renderer& renderer, const Font& font, const std::u16string& text, const Colour& colour)
		{
			SDL_Surface* renderedTextSurface = TTF_RenderUNICODE_Blended(font.GetRawHandle(), reinterpret_cast<const std::uint16_t*>(text.data()), colour);

			if (renderedTextSurface == nullptr)
			{
				return Texture();
			}

			Texture textTexture = Texture(renderer, renderedTextSurface);
			SDL_FreeSurface(renderedTextSurface);
			renderedTextSurface = nullptr;

			return textTexture;
		}

		[[nodiscard]] Texture RenderTextWrapped(const Renderer& renderer, const Font& font, const std::string& text, const Colour& colour, const unsigned int wrapLength)
		{
			SDL_Surface* renderedTextSurface = TTF_RenderText_Blended_Wrapped(font.GetRawHandle(), text.c_str(), colour, wrapLength);

			if (renderedTextSurface == nullptr)
			{
				return Texture();
			}

			Texture textTexture = Texture(renderer, renderedTextSurface);
			SDL_FreeSurface(renderedTextSurface);
			renderedTextSurface = nullptr;

			return textTexture;
		}

		[[nodiscard]] Texture RenderTextWrapped(const Renderer& renderer, const Font& font, const std::u16string& text, const Colour& colour, const unsigned int wrapLength)
		{
			SDL_Surface* renderedTextSurface = TTF_RenderUNICODE_Blended_Wrapped(font.GetRawHandle(), reinterpret_cast<const std::uint16_t*>(text.data()), colour, wrapLength);

			if (renderedTextSurface == nullptr)
			{
				return Texture();
			}

			Texture textTexture = Texture(renderer, renderedTextSurface);
			SDL_FreeSurface(renderedTextSurface);
			renderedTextSurface = nullptr;

			return textTexture;
		}

		[[nodiscard]] Texture RenderGlyphQuick(const Renderer& renderer, const Font& font, const char glyph, const Colour& colour)
		{
			SDL_Surface* renderedTextSurface = TTF_RenderGlyph_Solid(font.GetRawHandle(), glyph, colour);

			if (renderedTextSurface == nullptr)
			{
				return Texture();
			}

			Texture textTexture = Texture(renderer, renderedTextSurface);
			SDL_FreeSurface(renderedTextSurface);
			renderedTextSurface = nullptr;

			return textTexture;
		}

		[[nodiscard]] Texture RenderGlyphQuick(const Renderer& renderer, const Font& font, const char16_t glyph, const Colour& colour)
		{
			SDL_Surface* renderedTextSurface = TTF_RenderGlyph_Solid(font.GetRawHandle(), static_cast<std::uint16_t>(glyph), colour);

			if (renderedTextSurface == nullptr)
			{
				return Texture();
			}

			Texture textTexture = Texture(renderer, renderedTextSurface);
			SDL_FreeSurface(renderedTextSurface);
			renderedTextSurface = nullptr;

			return textTexture;
		}

		[[nodiscard]] Texture RenderTextQuick(const Renderer& renderer, const Font& font, const std::string& text, const Colour& colour)
		{
			SDL_Surface* renderedTextSurface = TTF_RenderText_Solid(font.GetRawHandle(), text.c_str(), colour);

			if (renderedTextSurface == nullptr)
			{
				return Texture();
			}

			Texture textTexture = Texture(renderer, renderedTextSurface);
			SDL_FreeSurface(renderedTextSurface);
			renderedTextSurface = nullptr;

			return textTexture;
		}

		[[nodiscard]] Texture RenderTextQuick(const Renderer& renderer, const Font& font, const std::u16string& text, const Colour& colour)
		{
			SDL_Surface* renderedTextSurface = TTF_RenderUNICODE_Solid(font.GetRawHandle(), reinterpret_cast<const std::uint16_t*>(text.data()), colour);

			if (renderedTextSurface == nullptr)
			{
				return Texture();
			}

			Texture textTexture = Texture(renderer, renderedTextSurface);
			SDL_FreeSurface(renderedTextSurface);
			renderedTextSurface = nullptr;

			return textTexture;
		}

		[[nodiscard]] Texture RenderShadedGlyph(const Renderer& renderer, const Font& font, const char glyph, const Colour& colour, const Colour& backgroundColour)
		{
			SDL_Surface* renderedTextSurface = TTF_RenderGlyph_Shaded(font.GetRawHandle(), glyph, colour, backgroundColour);

			if (renderedTextSurface == nullptr)
			{
				return Texture();
			}

			Texture textTexture = Texture(renderer, renderedTextSurface);
			SDL_FreeSurface(renderedTextSurface);
			renderedTextSurface = nullptr;

			return textTexture;
		}

		[[nodiscard]] Texture RenderShadedGlyph(const Renderer& renderer, const Font& font, const char16_t glyph, const Colour& colour, const Colour& backgroundColour)
		{
			SDL_Surface* renderedTextSurface = TTF_RenderGlyph_Shaded(font.GetRawHandle(), static_cast<std::uint16_t>(glyph), colour, backgroundColour);

			if (renderedTextSurface == nullptr)
			{
				return Texture();
			}

			Texture textTexture = Texture(renderer, renderedTextSurface);
			SDL_FreeSurface(renderedTextSurface);
			renderedTextSurface = nullptr;

			return textTexture;
		}

		[[nodiscard]] Texture RenderShadedText(const Renderer& renderer, const Font& font, const std::string& text, const Colour& colour, const Colour& backgroundColour)
		{
			SDL_Surface* renderedTextSurface = TTF_RenderText_Shaded(font.GetRawHandle(), text.c_str(), colour, backgroundColour);

			if (renderedTextSurface == nullptr)
			{
				return Texture();
			}

			Texture textTexture = Texture(renderer, renderedTextSurface);
			SDL_FreeSurface(renderedTextSurface);
			renderedTextSurface = nullptr;

			return textTexture;
		}

		[[nodiscard]] Texture RenderShadedText(const Renderer& renderer, const Font& font, const std::u16string& text, const Colour& colour, const Colour& backgroundColour)
		{
			SDL_Surface* renderedTextSurface = TTF_RenderUNICODE_Shaded(font.GetRawHandle(), reinterpret_cast<const std::uint16_t*>(text.data()), colour, backgroundColour);

			if (renderedTextSurface == nullptr)
			{
				return Texture();
			}

			Texture textTexture = Texture(renderer, renderedTextSurface);
			SDL_FreeSurface(renderedTextSurface);
			renderedTextSurface = nullptr;

			return textTexture;
		}
	}
}