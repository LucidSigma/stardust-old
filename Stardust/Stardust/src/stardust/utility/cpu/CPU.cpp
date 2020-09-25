#include "CPU.h"

#include <SDL2/SDL.h>

namespace stardust
{
	namespace cpu
	{
		unsigned int GetL1CacheLineSize()
		{
			return static_cast<unsigned int>(SDL_GetCPUCacheLineSize());
		}

		unsigned int GetCPUCount()
		{
			return static_cast<unsigned int>(SDL_GetCPUCount());
		}

		unsigned int GetSystemRAMCount()
		{
			return static_cast<unsigned int>(SDL_GetSystemRAM());
		}
	}
}