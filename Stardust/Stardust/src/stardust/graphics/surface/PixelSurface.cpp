#include "PixelSurface.h"

#include <utility>

namespace stardust
{
	void PixelSurface::SurfaceDestroyer::operator()(SDL_Surface* const surface) const noexcept
	{
		SDL_FreeSurface(surface);
	}

	PixelSurface::PixelSurface(const unsigned int width, const unsigned int height, const unsigned int channelCount)
	{
		Initialise(width, height, channelCount);
	}

	PixelSurface::PixelSurface(const unsigned int width, const unsigned int height, const unsigned int channelCount, void* const pixels)
	{
		Initialise(width, height, channelCount, pixels);
	}

	PixelSurface::PixelSurface(PixelSurface&& other) noexcept
		: m_handle(nullptr), m_isLocked(false)
	{
		std::swap(m_handle, other.m_handle);
		std::swap(m_isLocked, other.m_isLocked);
	}

	PixelSurface& PixelSurface::operator=(PixelSurface&& other) noexcept
	{
		m_handle = std::exchange(other.m_handle, nullptr);
		m_isLocked = std::exchange(other.m_isLocked, false);

		return *this;
	}

	PixelSurface::~PixelSurface() noexcept
	{
		Destroy();
	}

	void PixelSurface::Initialise(const unsigned int width, const unsigned int height, const unsigned int channelCount)
	{
		constexpr std::uint32_t RedMask = SDL_BYTEORDER == SDL_LIL_ENDIAN ? 0x00'00'00'FF : 0xFF'00'00'00;
		constexpr std::uint32_t GreenMask = SDL_BYTEORDER == SDL_LIL_ENDIAN ? 0x00'00'FF'00 : 0x00'FF'00'00;
		constexpr std::uint32_t BlueMask = SDL_BYTEORDER == SDL_LIL_ENDIAN ? 0x00'FF'00'00 : 0x00'00'FF'00;
		constexpr std::uint32_t AlphaMask = SDL_BYTEORDER == SDL_LIL_ENDIAN ? 0xFF'00'00'00 : 0x00'00'00'FF;

		m_handle = std::unique_ptr<SDL_Surface, SurfaceDestroyer>(
			SDL_CreateRGBSurface(
				0u,
				static_cast<int>(width),
				static_cast<int>(height),
				static_cast<int>(channelCount * 8u),
				RedMask,
				GreenMask,
				BlueMask,
				AlphaMask
			)
		);
	}

	void PixelSurface::Initialise(const unsigned int width, const unsigned int height, const unsigned int channelCount, void* const pixels)
	{
		constexpr std::uint32_t RedMask = SDL_BYTEORDER == SDL_LIL_ENDIAN ? 0x00'00'00'FF : 0xFF'00'00'00;
		constexpr std::uint32_t GreenMask = SDL_BYTEORDER == SDL_LIL_ENDIAN ? 0x00'00'FF'00 : 0x00'FF'00'00;
		constexpr std::uint32_t BlueMask = SDL_BYTEORDER == SDL_LIL_ENDIAN ? 0x00'FF'00'00 : 0x00'00'FF'00;
		constexpr std::uint32_t AlphaMask = SDL_BYTEORDER == SDL_LIL_ENDIAN ? 0xFF'00'00'00 : 0x00'00'00'FF;

		m_handle = std::unique_ptr<SDL_Surface, SurfaceDestroyer>(
			SDL_CreateRGBSurfaceFrom(
				pixels,
				static_cast<int>(width),
				static_cast<int>(height),
				static_cast<int>(channelCount * 8u),
				width * 4u,
				RedMask,
				GreenMask,
				BlueMask,
				AlphaMask
			)
		);
		
	}

	void PixelSurface::Destroy() noexcept
	{
		if (m_handle != nullptr)
		{
			Unlock();

			m_handle = nullptr;
		}
	}

	void PixelSurface::Lock()
	{
		m_isLocked = SDL_LockSurface(GetRawHandle()) == 0;
	}

	void PixelSurface::Unlock()
	{
		if (m_isLocked)
		{
			SDL_UnlockSurface(GetRawHandle());
			m_isLocked = false;
		}
	}

	void PixelSurface::Blit(const PixelSurface& source, const std::optional<rect::Rect>& sourceArea, const rect::Point& destinationPosition) const
	{
		const rect::Rect* sourceAreaPointer = sourceArea.has_value() ? &sourceArea.value() : nullptr;
		rect::Rect destinationRect{ destinationPosition.x, destinationPosition.y, 0, 0 };

		SDL_BlitSurface(source.GetRawHandle(), sourceAreaPointer, GetRawHandle(), &destinationRect);
	}

	[[nodiscard]] glm::uvec2 PixelSurface::GetSize() const
	{
		return glm::uvec2{
			GetRawHandle()->w,
			GetRawHandle()->h,
		};
	}

	unsigned int PixelSurface::GetPitch() const
	{
		return static_cast<unsigned int>(GetRawHandle()->pitch);
	}

	std::uint32_t PixelSurface::GetFormat() const
	{
		return GetRawHandle()->format->format;
	}

	void* PixelSurface::GetPixels() const
	{
		return GetRawHandle()->pixels;
	}
}