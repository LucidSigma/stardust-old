#include "VFS.h"

#include <filesystem>

#include <physfs/physfs.h>

namespace stardust
{
	namespace vfs
	{
		[[nodiscard]] bool Initialise(const char* argv0)
		{
			return PHYSFS_init(argv0) != 0;
		}

		void Quit()
		{
			PHYSFS_deinit();
		}

		void AddToSearchPath(const std::string_view& directory)
		{
			PHYSFS_mount(directory.data(), nullptr, 1);
		}

		void AddToSearchPath(const std::initializer_list<std::string_view>& directories)
		{
			for (const auto& directory : directories)
			{
				AddToSearchPath(directory);
			}
		}

		void SetWriteDirectory(const std::string_view& writeDirectory)
		{
			PHYSFS_setWriteDir(writeDirectory.data());
		}

		[[nodiscard]] std::string GetDirectorySeparator()
		{
			return std::string(PHYSFS_getDirSeparator());
		}

		[[nodiscard]] std::vector<std::string> GetAllFilesInDirectory(const std::string_view& directory)
		{
			std::vector<std::string> files{ };
			char** enumeratedFiles = PHYSFS_enumerateFiles(directory.data());

			for (const char* const* filename = enumeratedFiles; *filename != nullptr; ++filename)
			{
				files.push_back(std::string(directory) + "/" + *filename);
			}

			PHYSFS_freeList(enumeratedFiles);
			enumeratedFiles = nullptr;

			return files;
		}

		[[nodiscard]] std::vector<std::string> GetAllFileNamesInDirectory(const std::string_view& directory)
		{		
			std::vector<std::string> files{ };
			char** enumeratedFiles = PHYSFS_enumerateFiles(directory.data());
			
			for (const char* const* filename = enumeratedFiles; *filename != nullptr; ++filename)
			{
				files.push_back(std::string(*filename));
			}

			PHYSFS_freeList(enumeratedFiles);
			enumeratedFiles = nullptr;

			return files;
		}

		[[nodiscard]] bool DoesFileExist(const std::string_view& filepath)
		{
			return PHYSFS_exists(filepath.data()) != 0;
		}

		[[nodiscard]] std::string GetFilenameFromDirectory(const std::string_view& filepath)
		{
			return std::filesystem::path(filepath).filename().string();
		}

		[[nodiscard]] std::string GetFileStem(const std::string_view& filename)
		{
			return std::filesystem::path(filename).stem().string();
		}

		[[nodiscard]] std::string GetFileExtension(const std::string_view& filename)
		{
			return std::filesystem::path(filename).extension().string();
		}

		[[nodiscard]] std::vector<std::byte> ReadFileData(const std::string_view& filepath)
		{
			PHYSFS_File* file = PHYSFS_openRead(filepath.data());

			if (file == nullptr)
			{
				return std::vector<std::byte>{ };
			}

			const PHYSFS_sint64 fileSize = PHYSFS_fileLength(file);
			std::vector<std::byte> fileData(fileSize);

			if (PHYSFS_readBytes(file, fileData.data(), fileSize) == -1)
			{
				PHYSFS_close(file);
				file = nullptr;

				return std::vector<std::byte>{ };
			}

			PHYSFS_close(file);
			file = nullptr;

			return fileData;
		}

		Status WriteToFile(const std::string_view& filepath, const std::vector<std::byte>& data)
		{
			PHYSFS_File* file = PHYSFS_openWrite(filepath.data());

			if (file == nullptr)
			{
				return Status::Fail;
			}

			if (PHYSFS_writeBytes(file, data.data(), data.size()) == -1)
			{
				PHYSFS_close(file);
				file = nullptr;

				return Status::Fail;
			}

			PHYSFS_close(file);
			file = nullptr;

			return Status::Success;
		}

		Status WriteToFile(const std::string_view& filepath, const std::string& data)
		{
			PHYSFS_File* file = PHYSFS_openWrite(filepath.data());

			if (file == nullptr)
			{
				return Status::Fail;
			}

			if (PHYSFS_writeBytes(file, data.data(), data.length()) == -1)
			{
				PHYSFS_close(file);
				file = nullptr;

				return Status::Fail;
			}

			PHYSFS_close(file);
			file = nullptr;

			return Status::Success;
		}

		Status AppendToFile(const std::string_view& filepath, const std::vector<std::byte>& data)
		{
			PHYSFS_File* file = PHYSFS_openAppend(filepath.data());

			if (file == nullptr)
			{
				return Status::Fail;
			}

			if (PHYSFS_writeBytes(file, data.data(), data.size()) == -1)
			{
				PHYSFS_close(file);
				file = nullptr;

				return Status::Fail;
			}

			PHYSFS_close(file);
			file = nullptr;

			return Status::Success;
		}

		Status AppendToFile(const std::string_view& filepath, const std::string& data)
		{
			PHYSFS_File* file = PHYSFS_openAppend(filepath.data());

			if (file == nullptr)
			{
				return Status::Fail;
			}

			if (PHYSFS_writeBytes(file, data.data(), data.length()) == -1)
			{
				PHYSFS_close(file);
				file = nullptr;

				return Status::Fail;
			}

			PHYSFS_close(file);
			file = nullptr;

			return Status::Success;
		}
	}
}