#include "Texture.h"

#include <cstddef>
#include <utility>
#include <vector>

#include <stb/stb_image.h>

#include "../../vfs/VFS.h"

namespace stardust
{
	void Texture::TextureDestroyer::operator()(SDL_Texture* const texture) const noexcept
	{
		SDL_DestroyTexture(texture);
	}

	Texture::Texture(const Renderer& renderer, const std::string_view& filepath)
	{
		Initialise(renderer, filepath);
	}

	Texture::Texture(const Renderer& renderer, const std::uint32_t format, const AccessType accessType, const glm::uvec2& size)
	{
		Initialise(renderer, format, accessType, size);
	}

	Texture::Texture(Texture&& other) noexcept
		: m_renderer(nullptr), m_handle(nullptr), m_size(glm::uvec2{ 0u, 0u }), m_format(0u), m_accessType(AccessType::Static), m_isLocked(false)
	{
		std::swap(m_renderer, other.m_renderer);
		std::swap(m_handle, other.m_handle);

		std::swap(m_size, other.m_size);
		std::swap(m_format, other.m_format);
		std::swap(m_accessType, other.m_accessType);

		std::swap(m_isLocked, other.m_isLocked);
	}

	Texture& Texture::operator =(Texture&& other) noexcept
	{
		m_renderer = std::exchange(other.m_renderer, nullptr);
		m_handle = std::exchange(other.m_handle, nullptr);

		m_size = std::exchange(other.m_size, glm::uvec2{ 0u, 0u });
		m_format = std::exchange(other.m_format, 0u);
		m_accessType = std::exchange(other.m_accessType, AccessType::Static);

		m_isLocked = std::exchange(other.m_isLocked, false);

		return *this;
	}

	Texture::~Texture() noexcept
	{
		Destroy();
	}

	void Texture::Initialise(const Renderer& renderer, const std::string_view& filepath)
	{
		const std::vector<std::byte> rawTextureData = vfs::ReadFileData(filepath);

		if (rawTextureData.empty())
		{
			return;
		}

		int width = 0;
		int height = 0;
		int channelCount = 0;
		stbi_uc* textureData = stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(rawTextureData.data()), static_cast<int>(rawTextureData.size()), &width, &height, &channelCount, STBI_rgb_alpha);

		if (textureData == nullptr)
		{
			return;
		}

		constexpr std::uint32_t RedMask = SDL_BYTEORDER == SDL_LIL_ENDIAN ? 0x00'00'00'FF : 0xFF'00'00'00;
		constexpr std::uint32_t GreenMask = SDL_BYTEORDER == SDL_LIL_ENDIAN ? 0x00'00'FF'00 : 0x00'FF'00'00;
		constexpr std::uint32_t BlueMask = SDL_BYTEORDER == SDL_LIL_ENDIAN ? 0x00'FF'00'00 : 0x00'00'FF'00;
		constexpr std::uint32_t AlphaMask = SDL_BYTEORDER == SDL_LIL_ENDIAN ? 0xFF'00'00'00 : 0x00'00'00'FF;

		SDL_Surface* textureSurface = SDL_CreateRGBSurfaceFrom(textureData, width, height, sizeof(std::uint32_t) * 8u, width * channelCount, RedMask, GreenMask, BlueMask, AlphaMask);

		if (textureData == nullptr)
		{
			stbi_image_free(textureData);
			textureData = nullptr;

			return;
		}

		m_handle = std::unique_ptr<SDL_Texture, TextureDestroyer>(SDL_CreateTextureFromSurface(renderer.GetRawHandle(), textureSurface));
		
		stbi_image_free(textureData);
		textureData = nullptr;
		SDL_FreeSurface(textureSurface);
		textureSurface = nullptr;

		if (m_handle != nullptr)
		{
			m_renderer = &renderer;
			m_size = glm::uvec2{ width, height };

			int accessType = 0;
			SDL_QueryTexture(GetRawHandle(), &m_format, &accessType, nullptr, nullptr);
			m_accessType = static_cast<AccessType>(accessType);
		}
	}

	void Texture::Initialise(const Renderer& renderer, const std::uint32_t format, const AccessType accessType, const glm::uvec2& size)
	{
		m_handle = std::unique_ptr<SDL_Texture, TextureDestroyer>(
			SDL_CreateTexture(
				renderer.GetRawHandle(),
				format,
				static_cast<int>(accessType),
				static_cast<int>(size.x),
				static_cast<int>(size.y)
			)
		);

		if (m_handle != nullptr)
		{
			m_renderer = &renderer;

			m_size = size;
			m_format = format;
			m_accessType = accessType;
		}
	}

	void Texture::Destroy() noexcept
	{
		if (m_handle != nullptr)
		{
			if (m_renderer->GetCurrentRenderTarget() == this)
			{
				m_renderer->ResetRenderTarget();
			}

			m_renderer = nullptr;
			m_handle = nullptr;

			m_size = glm::uvec2{ 0u, 0u };
			m_format = 0u;
			m_accessType = AccessType::Static;
		}
	}

	void Texture::Lock(const std::optional<rect::Rect>& lockingArea, void*& out_pixels, unsigned int& out_pitch)
	{
		if (m_accessType == AccessType::Streaming)
		{
			const rect::Rect* lockingAreaPointer = lockingArea.has_value() ? &lockingArea.value() : nullptr;

			if (SDL_LockTexture(GetRawHandle(), lockingAreaPointer, &out_pixels, reinterpret_cast<int*>(&out_pitch)) == 0)
			{
				m_isLocked = true;
			}
		}
	}

	void Texture::Unlock()
	{
		if (m_accessType == AccessType::Streaming)
		{
			SDL_UnlockTexture(GetRawHandle());
			m_isLocked = false;
		}
	}

	void Texture::UpdateArea(const std::optional<rect::Rect>& areaToUpdate, const void* const pixels, const unsigned int pixelRowLength)
	{
		const rect::Rect* updateAreaPointer = areaToUpdate.has_value() ? &areaToUpdate.value() : nullptr;

		SDL_UpdateTexture(GetRawHandle(), updateAreaPointer, pixels, static_cast<int>(pixelRowLength));
	}

	Texture::ScaleMode Texture::GetScaleMode() const noexcept
	{
		SDL_ScaleMode scaleMode = SDL_ScaleModeNearest;
		SDL_GetTextureScaleMode(GetRawHandle(), &scaleMode);

		return static_cast<ScaleMode>(scaleMode);
	}

	void Texture::SetScaleMode(const ScaleMode scaleMode) const noexcept
	{
		SDL_SetTextureScaleMode(GetRawHandle(), static_cast<SDL_ScaleMode>(scaleMode));
	}

	[[nodiscard]] Renderer::BlendMode Texture::GetBlendMode() const noexcept
	{
		SDL_BlendMode blendMode = SDL_BLENDMODE_INVALID;
		SDL_GetTextureBlendMode(GetRawHandle(), &blendMode);

		return static_cast<Renderer::BlendMode>(blendMode);
	}

	void Texture::SetBlendMode(const Renderer::BlendMode blendMode) const noexcept
	{
		SDL_SetTextureBlendMode(GetRawHandle(), static_cast<SDL_BlendMode>(blendMode));
	}

	[[nodiscard]] std::uint8_t Texture::GetAlphaMod() const noexcept
	{
		std::uint8_t alphaMod = 0u;
		SDL_GetTextureAlphaMod(GetRawHandle(), &alphaMod);

		return alphaMod;
	}

	void Texture::SetAlphaMod(const std::uint8_t alphaMod) const noexcept
	{
		SDL_SetTextureAlphaMod(GetRawHandle(), alphaMod);
	}
	
	[[nodiscard]] std::tuple<std::uint8_t, std::uint8_t, std::uint8_t> Texture::GetColourMod() const noexcept
	{
		std::uint8_t red = 0u;
		std::uint8_t green = 0u;
		std::uint8_t blue = 0u;

		SDL_GetTextureColorMod(GetRawHandle(), &red, &green, &blue);

		return { red, green, blue };
	}

	void Texture::SetColourMod(const std::uint8_t red, const std::uint8_t green, const std::uint8_t blue) const noexcept
	{
		SDL_SetTextureColorMod(GetRawHandle(), red, green, blue);
	}
}