#pragma once
#ifndef CLIPBOARD_H
#define CLIPBOARD_H

#include <string>

namespace stardust
{
	namespace clipboard
	{
		extern bool HasText();

		[[nodiscard]] extern std::string GetText();
		extern void SetText(const std::string& text);
	}
}

#endif CLIPBOARD_H