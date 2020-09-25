#include "MessageBox.h"

#include <cstdint>

namespace stardust
{
	namespace message_box
	{
		void Show(const std::string_view& title, const std::string_view& message, const Type type)
		{
			SDL_ShowSimpleMessageBox(static_cast<std::uint32_t>(type), title.data(), message.data(), nullptr);
		}
	}
}
