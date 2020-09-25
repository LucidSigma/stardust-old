#pragma once
#ifndef CONFIG_H
#define CONFIG_H

#include <string_view>

#include <nlohmann/json.hpp>

#include "../utility/enums/Enums.h"

namespace stardust
{
	class Config
	{
	private:
		std::string_view m_filepath;
		nlohmann::json m_data;

	public:
		Config() = default;
		~Config() noexcept = default;

		[[nodiscard]] Status Initialise(const std::string_view& configFilepath);
		void Save() const;

		inline auto& operator [](const std::string_view& configType) { return m_data[configType.data()]; }
	};
}

#endif