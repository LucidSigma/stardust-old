#pragma once
#ifndef COLOUR_H
#define COLOUR_H

#include <SDL2/SDL.h>

namespace stardust
{
	using Colour = SDL_Color;

	namespace colours
	{
		constexpr Colour Black{ 0u, 0u, 0u, 255u };
		constexpr Colour Red{ 255u, 0u, 0u, 255u };
		constexpr Colour Lime{ 0u, 255u, 0u, 255u };
		constexpr Colour Blue{ 0u, 0u, 255u, 255u };
		constexpr Colour Yellow{ 255u, 255u, 0u, 255u };
		constexpr Colour Magenta{ 255u, 0u, 255u, 255u };
		constexpr Colour Cyan{ 255u, 255u, 255u, 255u };
		constexpr Colour White{ 255u, 255u, 255u, 255u };

		constexpr Colour Silver{ 192u, 192u, 192u, 255u };
		constexpr Colour Grey{ 128u, 128u, 128u, 255u };
		constexpr Colour Maroon{ 128u, 0u, 0u, 255u };
		constexpr Colour Olive{ 128u, 128u, 0u, 255u };
		constexpr Colour Green{ 0u, 128u, 0u, 255u };
		constexpr Colour Teal{ 0u, 128u, 128u, 255u };
		constexpr Colour Navy{ 0u, 0u, 128u, 255u };
		constexpr Colour Purple{ 128u, 0u, 128u, 255u };

		constexpr Colour Orange{ 255u, 128u, 0u, 255u };
		constexpr Colour Brown{ 150u, 75u, 0u, 255u };
		constexpr Colour Pink{ 255u, 192u, 203u, 255u };
		constexpr Colour Tan{ 210u, 180u, 140u, 255u };
	}
}

#endif