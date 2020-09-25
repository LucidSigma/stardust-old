#include "Display.h"

namespace stardust
{
	namespace display
	{
		[[nodiscard]] unsigned int GetVideoDisplayCount()
		{
			return static_cast<unsigned int>(SDL_GetNumVideoDisplays());
		}

		[[nodiscard]] unsigned int GetVideoDriverCount()
		{
			return static_cast<unsigned int>(SDL_GetNumVideoDrivers());
		}

		[[nodiscard]] DisplayData GetDisplayData(const int displayIndex)
		{
			DisplayData displayData{ };

			displayData.name = SDL_GetDisplayName(displayIndex);

			SDL_DisplayMode displayMode{ };
			SDL_GetCurrentDisplayMode(displayIndex, &displayMode);

			displayData.size = glm::uvec2{ displayMode.w, displayMode.h };
			displayData.refreshRate = displayMode.refresh_rate;
			displayData.format = displayMode.format;

			SDL_GetDisplayBounds(displayIndex, &displayData.bounds);
			SDL_GetDisplayUsableBounds(displayIndex, &displayData.usableBounds);

			displayData.orientation = static_cast<DisplayOrientation>(SDL_GetDisplayOrientation(displayIndex));
		
			displayData.verticalDPI = 0;
			displayData.horizontalDPI = 0;
			displayData.diagonalDPI = 0;
			SDL_GetDisplayDPI(displayIndex, &displayData.diagonalDPI, &displayData.horizontalDPI, &displayData.verticalDPI);

			return displayData;
		}
	}
}