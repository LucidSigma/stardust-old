#pragma once
#ifndef LOG_H
#define LOG_H

#include "../../utility/interfaces/INoncopyable.h"
#include "../../utility/interfaces/INonmovable.h"

#include <memory>
#include <string>
#include <string_view>
#include <utility>

#include <spdlog/spdlog.h>

namespace stardust
{
	class Log
		: private INoncopyable, private INonmovable
	{
	private:
		inline static std::shared_ptr<spdlog::logger> s_engineLogger = nullptr;
		inline static std::shared_ptr<spdlog::logger> s_clientLogger = nullptr;

	public:
		static void Initialise(const std::string_view& logFilepath);

		template <typename... Args>
		static void EngineTrace(const std::string& message, Args&&... args)
		{
		#ifndef NDEBUG
			s_engineLogger->trace(message, std::forward<Args>(args)...);
		#endif
		}

		template <typename... Args>
		static void EngineDebug(const std::string& message, Args&&... args)
		{
		#ifndef NDEBUG
			s_engineLogger->debug(message, std::forward<Args>(args)...);
		#endif
		}

		template <typename... Args>
		static void EngineInfo(const std::string& message, Args&&... args)
		{
		#ifndef NDEBUG
			s_engineLogger->info(message, std::forward<Args>(args)...);
		#endif
		}

		template <typename... Args>
		static void EngineWarn(const std::string& message, Args&&... args)
		{
		#ifndef NDEBUG
			s_engineLogger->warn(message, std::forward<Args>(args)...);
		#endif
		}

		template <typename... Args>
		static void EngineError(const std::string& message, Args&&... args)
		{
		#ifndef NDEBUG
			s_engineLogger->error(message, std::forward<Args>(args)...);
		#endif
		}

		template <typename... Args>
		static void EngineCritical(const std::string& message, Args&&... args)
		{
		#ifndef NDEBUG
			s_engineLogger->critical(message, std::forward<Args>(args)...);
		#endif
		}

		template <typename... Args>
		static void Trace(const std::string& message, Args&&... args)
		{
		#ifndef NDEBUG
			s_clientLogger->trace(message, std::forward<Args>(args)...);
		#endif
		}

		template <typename... Args>
		static void Debug(const std::string& message, Args&&... args)
		{
		#ifndef NDEBUG
			s_clientLogger->debug(message, std::forward<Args>(args)...);
		#endif
		}

		template <typename... Args>
		static void Info(const std::string& message, Args&&... args)
		{
		#ifndef NDEBUG
			s_clientLogger->info(message, std::forward<Args>(args)...);
		#endif
		}

		template <typename... Args>
		static void Warn(const std::string& message, Args&&... args)
		{
		#ifndef NDEBUG
			s_clientLogger->warn(message, std::forward<Args>(args)...);
		#endif
		}

		template <typename... Args>
		static void Error(const std::string& message, Args&&... args)
		{
		#ifndef NDEBUG
			s_clientLogger->error(message, std::forward<Args>(args)...);
		#endif
		}

		template <typename... Args>
		static void Critical(const std::string& message, Args&&... args)
		{
		#ifndef NDEBUG
			s_clientLogger->critical(message, std::forward<Args>(args)...);
		#endif
		}

		Log() = delete;
		~Log() noexcept = delete;
	};
}

#endif