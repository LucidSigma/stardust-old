#pragma once
#ifndef LOCALE_H
#define LOCALE_H

#include <optional>
#include <string>
#include <string_view>

#include <nlohmann/json.hpp>

#include "../utility/enums/Enums.h"

namespace stardust
{
	class Locale
	{
	private:
		std::string m_engineLocaleDirectory;
		std::string m_clientLocaleDirectory;

		nlohmann::json m_currentLocale;
		std::string m_currentLocaleName;

	public:
		Locale() = default;
		~Locale() noexcept = default;

		void Initialise(const std::string_view& engineLocaleDirectory, const std::string_view& clientLocaleDirectory);
		[[nodiscard]] Status SetLocale(const std::string_view& localeName);

		inline const std::string& GetCurrentLocaleName() const noexcept { return m_currentLocaleName; }
		inline const nlohmann::json& operator [](const std::string_view& localeString) const { return m_currentLocale[localeString.data()]; }

	private:
		[[nodiscard]] std::optional<nlohmann::json> LoadLocaleFile(const std::string& filepath) const;
	};
}

#endif