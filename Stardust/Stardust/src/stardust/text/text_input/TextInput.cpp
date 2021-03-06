#include "TextInput.h"

#include <algorithm>
#include <iterator>

namespace stardust
{
	namespace text_input
	{
		void Start()
		{
			if (!IsActive())
			{
				SDL_StartTextInput();
			}
		}

		void Stop()
		{
			if (IsActive())
			{
				SDL_StopTextInput();
			}
		}

		[[nodiscard]] bool IsActive()
		{
			return static_cast<bool>(SDL_IsTextInputActive());
		}

		[[nodiscard]] bool IsModStateActive(const ModState modState)
		{
			return (SDL_GetModState() & static_cast<SDL_Keymod>(modState)) == static_cast<SDL_Keymod>(modState);
		}

		[[nodiscard]] bool IsAnyModStateActive(const std::vector<ModState>& modStates)
		{
			return std::any_of(std::cbegin(modStates), std::cend(modStates), [](const ModState modState)
			{
				return IsModStateActive(modState);
			});
		}

		[[nodiscard]] bool AreAllModStatesActive(const std::vector<ModState>& modStates)
		{
			return std::all_of(std::cbegin(modStates), std::cend(modStates), [](const ModState modState)
			{
				return IsModStateActive(modState);
			});
		}
	}
}