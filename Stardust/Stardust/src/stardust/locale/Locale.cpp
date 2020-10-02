#include "Locale.h"

#include <cstddef>
#include <utility>
#include <vector>

#include "../vfs/VFS.h"

namespace stardust
{
	void Locale::Initialise(const std::string_view& engineLocaleDirectory, const std::string_view& clientLocaleDirectory)
	{
		m_engineLocaleDirectory = engineLocaleDirectory;
		m_clientLocaleDirectory = clientLocaleDirectory;
	}

	[[nodiscard]] Status Locale::SetLocale(const std::string_view& localeName)
	{
		const std::string engineLocaleFilepath = m_engineLocaleDirectory + "/" + std::string(localeName) + ".json";
		auto localeAccumulator = LoadLocaleFile(engineLocaleFilepath);

		if (!localeAccumulator.has_value())
		{
			return Status::Fail;
		}

		const std::string clientLocaleFilepath = m_clientLocaleDirectory + "/" + std::string(localeName) + ".json";

		if (vfs::DoesFileExist(clientLocaleFilepath))
		{
			const auto clientLocale = LoadLocaleFile(clientLocaleFilepath);

			if (!clientLocale.has_value())
			{
				return Status::Fail;
			}

			localeAccumulator->merge_patch(*clientLocale);

			if (localeAccumulator->is_discarded())
			{
				return Status::Fail;
			}
		}

		m_currentLocale = std::move(*localeAccumulator);
		m_currentLocaleName = localeName;

		return Status::Success;
	}

	[[nodiscard]] std::optional<nlohmann::json> Locale::LoadLocaleFile(const std::string& filepath) const
	{
		const std::vector<std::byte> localeData = vfs::ReadFileData(filepath);

		if (localeData.empty())
		{
			return std::nullopt;
		}

		const nlohmann::json locale = nlohmann::json::parse(
			reinterpret_cast<const unsigned char*>(localeData.data()),
			reinterpret_cast<const unsigned char*>(localeData.data()) + localeData.size(),
			nullptr,
			false
		);

		if (locale.is_discarded())
		{
			return std::nullopt;
		}
		else
		{
			return locale;
		}
	}
}