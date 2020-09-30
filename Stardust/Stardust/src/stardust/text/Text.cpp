#include "Text.h"

#include <cstdint>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "../rect/Rect.h"

namespace stardust
{
	namespace text
	{
		[[nodiscard]] Texture RenderGlyph(const Renderer& renderer, const Font& font, const char glyph, const Colour& colour)
		{
			return RenderGlyph(renderer, font, static_cast<char16_t>(glyph), colour);
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
			return RenderGlyphQuick(renderer, font, static_cast<char16_t>(glyph), colour);
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
			return RenderShadedGlyph(renderer, font, static_cast<char16_t>(glyph), colour, backgroundColour);
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

		[[nodiscard]] Texture RenderGlyphWithOutline(const Renderer& renderer, const Font& font, const char glyph, const Colour& colour, const unsigned int outlineSize, const Colour& outlineColour)
		{
			return RenderGlyphWithOutline(renderer, font, static_cast<char16_t>(glyph), colour, outlineSize, outlineColour);
		}

		[[nodiscard]] Texture RenderGlyphWithOutline(const Renderer& renderer, const Font& font, const char16_t glyph, const Colour& colour, const unsigned int outlineSize, const Colour& outlineColour)
		{
			font.SetOutlineThickness(outlineSize);
			SDL_Surface* renderedTextSurface = TTF_RenderGlyph_Blended(font.GetRawHandle(), static_cast<std::uint16_t>(glyph), outlineColour);
			font.RemoveOutline();

			if (renderedTextSurface == nullptr)
			{
				return Texture();
			}

			SDL_Surface* innerTextSurface = TTF_RenderGlyph_Blended(font.GetRawHandle(), static_cast<std::uint16_t>(glyph), colour);

			if (renderedTextSurface == nullptr)
			{
				return Texture();
			}

			rect::Rect blitArea = rect::Create(outlineSize, outlineSize, innerTextSurface->w, innerTextSurface->h);

			if (SDL_BlitSurface(innerTextSurface, nullptr, renderedTextSurface, &blitArea) != 0)
			{
				SDL_FreeSurface(renderedTextSurface);
				SDL_FreeSurface(innerTextSurface);
				renderedTextSurface = nullptr;
				innerTextSurface = nullptr;

				return Texture();
			}

			Texture textTexture = Texture(renderer, renderedTextSurface);

			SDL_FreeSurface(renderedTextSurface);
			SDL_FreeSurface(innerTextSurface);
			renderedTextSurface = nullptr;
			innerTextSurface = nullptr;

			return textTexture;
		}

		[[nodiscard]] Texture RenderTextWithOutline(const Renderer& renderer, const Font& font, const std::string& text, const Colour& colour, const unsigned int outlineSize, const Colour& outlineColour)
		{
			font.SetOutlineThickness(outlineSize);
			SDL_Surface* renderedTextSurface = TTF_RenderText_Blended(font.GetRawHandle(), text.c_str(), outlineColour);
			font.RemoveOutline();

			if (renderedTextSurface == nullptr)
			{
				return Texture();
			}

			SDL_Surface* innerTextSurface = TTF_RenderText_Blended(font.GetRawHandle(), text.c_str(), colour);

			if (renderedTextSurface == nullptr)
			{
				return Texture();
			}

			rect::Rect blitArea = rect::Create(outlineSize, outlineSize, innerTextSurface->w, innerTextSurface->h);

			if (SDL_BlitSurface(innerTextSurface, nullptr, renderedTextSurface, &blitArea) != 0)
			{
				SDL_FreeSurface(renderedTextSurface);
				SDL_FreeSurface(innerTextSurface);
				renderedTextSurface = nullptr;
				innerTextSurface = nullptr;

				return Texture();
			}

			Texture textTexture = Texture(renderer, renderedTextSurface);

			SDL_FreeSurface(renderedTextSurface);
			SDL_FreeSurface(innerTextSurface);
			renderedTextSurface = nullptr;
			innerTextSurface = nullptr;

			return textTexture;
		}

		[[nodiscard]] Texture RenderTextWithOutline(const Renderer& renderer, const Font& font, const std::u16string& text, const Colour& colour, const unsigned int outlineSize, const Colour& outlineColour)
		{
			font.SetOutlineThickness(outlineSize);
			SDL_Surface* renderedTextSurface = TTF_RenderUNICODE_Blended(font.GetRawHandle(), reinterpret_cast<const std::uint16_t*>(text.data()), outlineColour);
			font.RemoveOutline();

			if (renderedTextSurface == nullptr)
			{
				return Texture();
			}

			SDL_Surface* innerTextSurface = TTF_RenderUNICODE_Blended(font.GetRawHandle(), reinterpret_cast<const std::uint16_t*>(text.data()), colour);

			if (renderedTextSurface == nullptr)
			{
				return Texture();
			}

			rect::Rect blitArea = rect::Create(outlineSize, outlineSize, innerTextSurface->w, innerTextSurface->h);

			if (SDL_BlitSurface(innerTextSurface, nullptr, renderedTextSurface, &blitArea) != 0)
			{
				SDL_FreeSurface(renderedTextSurface);
				SDL_FreeSurface(innerTextSurface);
				renderedTextSurface = nullptr;
				innerTextSurface = nullptr;

				return Texture();
			}

			Texture textTexture = Texture(renderer, renderedTextSurface);

			SDL_FreeSurface(renderedTextSurface);
			SDL_FreeSurface(innerTextSurface);
			renderedTextSurface = nullptr;
			innerTextSurface = nullptr;

			return textTexture;
		}

		[[nodiscard]] Texture RenderTextWrappedWithOutline(const Renderer& renderer, const Font& font, const std::string& text, const Colour& colour, const unsigned int outlineSize, const Colour& outlineColour, const unsigned int wrapLength)
		{
			font.SetOutlineThickness(outlineSize);
			SDL_Surface* renderedTextSurface = TTF_RenderText_Blended_Wrapped(font.GetRawHandle(), text.c_str(), outlineColour, wrapLength);
			font.RemoveOutline();

			if (renderedTextSurface == nullptr)
			{
				return Texture();
			}

			SDL_Surface* innerTextSurface = TTF_RenderText_Blended_Wrapped(font.GetRawHandle(), text.c_str(), colour, wrapLength);

			if (renderedTextSurface == nullptr)
			{
				return Texture();
			}

			rect::Rect blitArea = rect::Create(outlineSize, outlineSize, innerTextSurface->w, innerTextSurface->h);

			if (SDL_BlitSurface(innerTextSurface, nullptr, renderedTextSurface, &blitArea) != 0)
			{
				SDL_FreeSurface(renderedTextSurface);
				SDL_FreeSurface(innerTextSurface);
				renderedTextSurface = nullptr;
				innerTextSurface = nullptr;

				return Texture();
			}

			Texture textTexture = Texture(renderer, renderedTextSurface);

			SDL_FreeSurface(renderedTextSurface);
			SDL_FreeSurface(innerTextSurface);
			renderedTextSurface = nullptr;
			innerTextSurface = nullptr;

			return textTexture;
		}

		[[nodiscard]] Texture RenderTextWrappedWithOutline(const Renderer& renderer, const Font& font, const std::u16string& text, const Colour& colour, const unsigned int outlineSize, const Colour& outlineColour, const unsigned int wrapLength)
		{
			font.SetOutlineThickness(outlineSize);
			SDL_Surface* renderedTextSurface = TTF_RenderUNICODE_Blended_Wrapped(font.GetRawHandle(), reinterpret_cast<const std::uint16_t*>(text.data()), outlineColour, wrapLength);
			font.RemoveOutline();

			if (renderedTextSurface == nullptr)
			{
				return Texture();
			}

			SDL_Surface* innerTextSurface = TTF_RenderUNICODE_Blended_Wrapped(font.GetRawHandle(), reinterpret_cast<const std::uint16_t*>(text.data()), colour, wrapLength);

			if (renderedTextSurface == nullptr)
			{
				return Texture();
			}

			rect::Rect blitArea = rect::Create(outlineSize, outlineSize, innerTextSurface->w, innerTextSurface->h);

			if (SDL_BlitSurface(innerTextSurface, nullptr, renderedTextSurface, &blitArea) != 0)
			{
				SDL_FreeSurface(renderedTextSurface);
				SDL_FreeSurface(innerTextSurface);
				renderedTextSurface = nullptr;
				innerTextSurface = nullptr;

				return Texture();
			}

			Texture textTexture = Texture(renderer, renderedTextSurface);

			SDL_FreeSurface(renderedTextSurface);
			SDL_FreeSurface(innerTextSurface);
			renderedTextSurface = nullptr;
			innerTextSurface = nullptr;

			return textTexture;
		}

		[[nodiscard]] Texture RenderGlyphQuickWithOutline(const Renderer& renderer, const Font& font, const char glyph, const Colour& colour, const unsigned int outlineSize, const Colour& outlineColour)
		{
			return RenderGlyphQuickWithOutline(renderer, font, static_cast<char16_t>(glyph), colour, outlineSize, outlineColour);
		}

		[[nodiscard]] Texture RenderGlyphQuickWithOutline(const Renderer& renderer, const Font& font, const char16_t glyph, const Colour& colour, const unsigned int outlineSize, const Colour& outlineColour)
		{
			font.SetOutlineThickness(outlineSize);
			SDL_Surface* renderedTextSurface = TTF_RenderGlyph_Solid(font.GetRawHandle(), static_cast<std::uint16_t>(glyph), outlineColour);
			font.RemoveOutline();

			if (renderedTextSurface == nullptr)
			{
				return Texture();
			}

			SDL_Surface* innerTextSurface = TTF_RenderGlyph_Solid(font.GetRawHandle(), static_cast<std::uint16_t>(glyph), colour);

			if (renderedTextSurface == nullptr)
			{
				return Texture();
			}

			rect::Rect blitArea = rect::Create(outlineSize, outlineSize, innerTextSurface->w, innerTextSurface->h);

			if (SDL_BlitSurface(innerTextSurface, nullptr, renderedTextSurface, &blitArea) != 0)
			{
				SDL_FreeSurface(renderedTextSurface);
				SDL_FreeSurface(innerTextSurface);
				renderedTextSurface = nullptr;
				innerTextSurface = nullptr;

				return Texture();
			}

			Texture textTexture = Texture(renderer, renderedTextSurface);

			SDL_FreeSurface(renderedTextSurface);
			SDL_FreeSurface(innerTextSurface);
			renderedTextSurface = nullptr;
			innerTextSurface = nullptr;

			return textTexture;
		}

		[[nodiscard]] Texture RenderTextQuickWithOutline(const Renderer& renderer, const Font& font, const std::string& text, const Colour& colour, const unsigned int outlineSize, const Colour& outlineColour)
		{
			font.SetOutlineThickness(outlineSize);
			SDL_Surface* renderedTextSurface = TTF_RenderText_Solid(font.GetRawHandle(), text.c_str(), outlineColour);
			font.RemoveOutline();

			if (renderedTextSurface == nullptr)
			{
				return Texture();
			}

			SDL_Surface* innerTextSurface = TTF_RenderText_Solid(font.GetRawHandle(), text.c_str(), colour);

			if (renderedTextSurface == nullptr)
			{
				return Texture();
			}

			rect::Rect blitArea = rect::Create(outlineSize, outlineSize, innerTextSurface->w, innerTextSurface->h);

			if (SDL_BlitSurface(innerTextSurface, nullptr, renderedTextSurface, &blitArea) != 0)
			{
				SDL_FreeSurface(renderedTextSurface);
				SDL_FreeSurface(innerTextSurface);
				renderedTextSurface = nullptr;
				innerTextSurface = nullptr;

				return Texture();
			}

			Texture textTexture = Texture(renderer, renderedTextSurface);

			SDL_FreeSurface(renderedTextSurface);
			SDL_FreeSurface(innerTextSurface);
			renderedTextSurface = nullptr;
			innerTextSurface = nullptr;

			return textTexture;
		}

		[[nodiscard]] Texture RenderTextQuickWithOutline(const Renderer& renderer, const Font& font, const std::u16string& text, const Colour& colour, const unsigned int outlineSize, const Colour& outlineColour)
		{
			font.SetOutlineThickness(outlineSize);
			SDL_Surface* renderedTextSurface = TTF_RenderUNICODE_Solid(font.GetRawHandle(), reinterpret_cast<const std::uint16_t*>(text.data()), outlineColour);
			font.RemoveOutline();

			if (renderedTextSurface == nullptr)
			{
				return Texture();
			}

			SDL_Surface* innerTextSurface = TTF_RenderUNICODE_Solid(font.GetRawHandle(), reinterpret_cast<const std::uint16_t*>(text.data()), colour);

			if (renderedTextSurface == nullptr)
			{
				return Texture();
			}

			rect::Rect blitArea = rect::Create(outlineSize, outlineSize, innerTextSurface->w, innerTextSurface->h);

			if (SDL_BlitSurface(innerTextSurface, nullptr, renderedTextSurface, &blitArea) != 0)
			{
				SDL_FreeSurface(renderedTextSurface);
				SDL_FreeSurface(innerTextSurface);
				renderedTextSurface = nullptr;
				innerTextSurface = nullptr;

				return Texture();
			}

			Texture textTexture = Texture(renderer, renderedTextSurface);

			SDL_FreeSurface(renderedTextSurface);
			SDL_FreeSurface(innerTextSurface);
			renderedTextSurface = nullptr;
			innerTextSurface = nullptr;

			return textTexture;
		}
	}
}