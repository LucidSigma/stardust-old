#pragma once
#ifndef VFS_H
#define VFS_H

#include <cstddef>
#include <initializer_list>
#include <string>
#include <string_view>
#include <vector>

#include "../utility/enums/Enums.h"

namespace stardust
{
	namespace vfs
	{
		[[nodiscard]] extern bool Initialise(const char* argv0);
		extern void Quit();

		extern void AddToSearchPath(const std::string_view& directory);
		extern void AddToSearchPath(const std::initializer_list<std::string_view>& directories);

		extern void SetWriteDirectory(const std::string_view& writeDirectory);

		[[nodiscard]] extern bool DoesFileExist(const std::string_view& filepath);

		[[nodiscard]] extern std::vector<std::byte> ReadFileData(const std::string_view& filepath);
		[[nodiscard]] extern Status WriteToFile(const std::string_view& filepath, const std::vector<std::byte>& data);
		[[nodiscard]] extern Status WriteToFile(const std::string_view& filepath, const std::string& data);
		[[nodiscard]] extern Status AppendToFile(const std::string_view& filepath, const std::vector<std::byte>& data);
		[[nodiscard]] extern Status AppendToFile(const std::string_view& filepath, const std::string& data);
	}
}

#endif