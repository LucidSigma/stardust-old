#pragma once
#ifndef DISPLAY_H
#define DISPLAY_H

#include <cstdint>
#include <string>
#include <type_traits>

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

#include "../../rect/Rect.h"

namespace stardust
{
	namespace display
	{
		enum class DisplayOrientation
			: std::underlying_type_t<SDL_DisplayOrientation>
		{
			Unknown = SDL_ORIENTATION_UNKNOWN,
			Landscape = SDL_ORIENTATION_LANDSCAPE,
			LandscapeFlipped = SDL_ORIENTATION_LANDSCAPE_FLIPPED,
			Portrait = SDL_ORIENTATION_PORTRAIT,
			PortraitFlipped = SDL_ORIENTATION_PORTRAIT_FLIPPED,
		};

		struct DisplayData
		{
			std::string name;

			glm::uvec2 size;
			unsigned int refreshRate;
			std::uint32_t format;

			rect::Rect bounds;
			rect::Rect usableBounds;

			DisplayOrientation orientation;

			float verticalDPI;
			float horizontalDPI;
			float diagonalDPI;
		};

		[[nodiscard]] extern unsigned int GetVideoDisplayCount();
		[[nodiscard]] extern unsigned int GetVideoDriverCount();

		[[nodiscard]] extern DisplayData GetDisplayData(const int displayIndex);
	}
}

#endif