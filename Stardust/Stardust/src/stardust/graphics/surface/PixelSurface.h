#pragma once
#ifndef PIXEL_SURFACE_H
#define PIXEL_SURFACE_H

#include "../../utility/interfaces/INoncopyable.h"

#include <cstdint>
#include <memory>
#include <optional>

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

#include "../../rect/Rect.h"

namespace stardust
{
	class PixelSurface
		: private INoncopyable
	{
	private:
		struct SurfaceDestroyer
		{
			void operator ()(SDL_Surface* const surface) const noexcept;
		};

		std::unique_ptr<SDL_Surface, SurfaceDestroyer> m_handle = nullptr;
		bool m_isLocked = false;

	public:
		PixelSurface() = default;
		PixelSurface(const unsigned int width, const unsigned int height);
		PixelSurface(const unsigned int width, const unsigned int height, void* const pixels);

		PixelSurface(PixelSurface&& other) noexcept;
		PixelSurface& operator =(PixelSurface&& other) noexcept;

		~PixelSurface() noexcept;

		void Initialise(const unsigned int width, const unsigned int height);
		void Initialise(const unsigned int width, const unsigned int height, void* const pixels);
		void Destroy() noexcept;

		inline bool IsValid() const noexcept { return m_handle != nullptr; }

		void Lock();
		void Unlock();
		inline bool IsLocked() const noexcept { return m_isLocked; }

		void Blit(const PixelSurface& source, const std::optional<rect::Rect>& sourceArea, const rect::Point& destinationPosition) const;

		[[nodiscard]] glm::uvec2 GetSize() const;
		unsigned int GetPitch() const;
		std::uint32_t GetFormat() const;
		void* GetPixels() const;

		[[nodiscard]] inline SDL_Surface* const GetRawHandle() const noexcept { return m_handle.get(); }
	};
}

#endif