#include "Clipboard.h"

#include <SDL2/SDL.h>

namespace stardust
{
    namespace clipboard
    {
        bool HasText()
        {
            return SDL_HasClipboardText();
        }

        [[nodiscard]] std::string GetText()
        {
            char* clipboardTextPointer = SDL_GetClipboardText();
            const std::string clipboardText(clipboardTextPointer);

            SDL_free(clipboardTextPointer);

            return clipboardText;
        }

        void SetText(const std::string& text)
        {
            SDL_SetClipboardText(text.c_str());
        }
    }
}