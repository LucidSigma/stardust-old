#include "Font.h"

#include <array>
#include <cstdint>
#include <utility>

#include "../../vfs/VFS.h"

namespace stardust
{
	void Font::FontDestroyer::operator()(TTF_Font* const font) const noexcept
	{
		TTF_CloseFont(font);
	}

	Font::Font(const std::string_view& fontFilepath, const unsigned int pointSize)
	{
		Initialise(fontFilepath, pointSize);
	}

	Font::Font(Font&& other) noexcept
		: m_handle(nullptr), m_pointSize(0u)
	{
		std::swap(m_handle, other.m_handle);
		std::swap(m_pointSize, other.m_pointSize);
	}

	Font& Font::operator =(Font&& other) noexcept
	{
		m_handle = std::exchange(other.m_handle, nullptr);
		m_pointSize = std::exchange(other.m_pointSize, 0u);

		return *this;
	}

	Font::~Font() noexcept
	{
		Destroy();
	}

	void Font::Initialise(const std::string_view& fontFilepath, const unsigned int pointSize)
	{
		m_fontFileData = vfs::ReadFileData(fontFilepath);
		
		if (m_fontFileData.empty())
		{
			return;
		}
		
		m_fontFileRWOps = SDL_RWFromConstMem(m_fontFileData.data(), static_cast<int>(m_fontFileData.size()));
		
		if (m_fontFileRWOps == nullptr)
		{
			return;
		}
		
		m_handle = std::unique_ptr<TTF_Font, FontDestroyer>(TTF_OpenFontRW(m_fontFileRWOps, SDL_FALSE, static_cast<int>(pointSize)));

		if (m_handle != nullptr)
		{
			m_pointSize = pointSize;
		}
	}

	void Font::Destroy() noexcept
	{
		if (m_handle != nullptr)
		{
			m_handle = nullptr;

			SDL_RWclose(m_fontFileRWOps);
			m_fontFileRWOps = nullptr;
			m_fontFileData.clear();

			m_pointSize = 0u;
		}
	}

	[[nodiscard]] std::vector<Font::Style> Font::GetStyles() const
	{
		constexpr std::array<Style, 4u> AllFontStyles{
			Style::Bold,
			Style::Italic,
			Style::Underline,
			Style::Strikethrough
		};

		const int styleBitmask = TTF_GetFontStyle(GetRawHandle());
		std::vector<Style> appliedStyles{ };

		for (const Style style : AllFontStyles)
		{
			if (styleBitmask & static_cast<int>(style))
			{
				appliedStyles.push_back(style);
			}
		}

		if (appliedStyles.empty())
		{
			appliedStyles.push_back(Style::Normal);
		}

		return appliedStyles;
	}

	void Font::SetStyles(const std::vector<Style>& styles) const
	{
		int styleBitmask = 0;

		for (const Style style : styles)
		{
			styleBitmask |= static_cast<int>(style);
		}

		TTF_SetFontStyle(GetRawHandle(), styleBitmask);
	}

	void Font::ClearStyles() const
	{
		TTF_SetFontStyle(GetRawHandle(), static_cast<int>(Style::Normal));
	}

	[[nodiscard]] unsigned int Font::GetOutlineThickness() const
	{
		return static_cast<unsigned int>(TTF_GetFontOutline(GetRawHandle()));
	}

	void Font::SetOutlineThickness(const unsigned int outlineThickness) const
	{
		TTF_SetFontOutline(GetRawHandle(), static_cast<int>(outlineThickness));
	}

	void Font::RemoveOutline() const
	{
		TTF_SetFontOutline(GetRawHandle(), 0);
	}
	
	[[nodiscard]] Font::Hinting Font::GetHinting() const
	{
		return static_cast<Hinting>(TTF_GetFontHinting(GetRawHandle()));
	}

	void Font::SetHinting(const Hinting hinting) const
	{
		TTF_SetFontHinting(GetRawHandle(), static_cast<int>(hinting));
	}

	[[nodiscard]] unsigned int Font::GetKerning() const
	{
		return static_cast<unsigned int>(TTF_GetFontKerning(GetRawHandle()));
	}

	void Font::SetKerning(const unsigned int kerning) const
	{
		TTF_SetFontKerning(GetRawHandle(), static_cast<int>(kerning));
	}

	[[nodiscard]] unsigned int Font::GetKerningBetweenGlyphs(const char16_t leftGlyph, const char16_t rightGlyph) const
	{
		return static_cast<unsigned int>(TTF_GetFontKerningSizeGlyphs(GetRawHandle(), static_cast<std::uint16_t>(leftGlyph), static_cast<std::uint16_t>(rightGlyph)));
	}

	[[nodiscard]] unsigned int Font::GetMaximumHeight() const
	{
		return static_cast<unsigned int>(TTF_FontHeight(GetRawHandle()));
	}

	[[nodiscard]] int Font::GetFontAscent() const
	{
		return TTF_FontAscent(GetRawHandle());
	}

	[[nodiscard]] int Font::GetFontDescent() const
	{
		return TTF_FontDescent(GetRawHandle());
	}

	[[nodiscard]] unsigned int Font::GetLineSkip() const
	{
		return static_cast<unsigned int>(TTF_FontLineSkip(GetRawHandle()));
	}

	[[nodiscard]] bool Font::IsFixedWidth() const
	{
		return static_cast<bool>(TTF_FontFaceIsFixedWidth(GetRawHandle()));
	}

	[[nodiscard]] std::optional<std::size_t> Font::GetGlyphIndex(const char16_t glyph) const
	{
		const int glyphIndex = TTF_GlyphIsProvided(GetRawHandle(), static_cast<std::uint16_t>(glyph));

		if (glyphIndex != 0)
		{
			return static_cast<std::size_t>(glyphIndex);
		}
		else
		{
			return std::nullopt;
		}
	}

	[[nodiscard]] bool Font::DoesGlyphExist(const char16_t glyph) const
	{
		return GetGlyphIndex(glyph).has_value();
	}

	[[nodiscard]] Font::GlyphMetrics Font::GetGlyphMetrics(const char16_t glyph) const
	{
		GlyphMetrics glyphMetrics{ };

		TTF_GlyphMetrics(
			GetRawHandle(),
			static_cast<std::uint16_t>(glyph),
			&glyphMetrics.minOffset.x,
			&glyphMetrics.maxOffset.x,
			&glyphMetrics.minOffset.y,
			&glyphMetrics.maxOffset.y,
			&glyphMetrics.advance
		);

		return glyphMetrics;
	}
	
	[[nodiscard]] glm::uvec2 Font::GetTextSize(const std::string& text) const
	{
		int textWidth = 0;
		int textHeight = 0;
		TTF_SizeUTF8(GetRawHandle(), text.c_str(), &textWidth, &textHeight);

		return glm::vec2{ textWidth, textHeight };
	}

	[[nodiscard]] glm::uvec2 Font::GetTextSize(const std::u16string& text) const
	{
		int textWidth = 0;
		int textHeight = 0;
		TTF_SizeUNICODE(GetRawHandle(), reinterpret_cast<const std::uint16_t*>(text.data()), &textWidth, &textHeight);

		return glm::vec2{ textWidth, textHeight };
	}
}