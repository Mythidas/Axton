#pragma once

#include "Axton/Core/Defines.h"

#include <spdlog/spdlog.h>

#include <memory>

namespace spdlog
{
	class logger;
}

namespace Axton
{
	// Client Console logging
	class AX_API Log
	{
	public:
		static void Construct();

		template <typename... Args>
		static void Trace(spdlog::format_string_t<Args...> message, Args &&...args)
		{
			s_ClientLogger->trace(message, std::forward<Args>(args)...);
		}
		template <typename... Args>
		static void Info(spdlog::format_string_t<Args...> message, Args &&...args)
		{
			s_ClientLogger->info(message, std::forward<Args>(args)...);
		}
		template <typename... Args>
		static void Warn(spdlog::format_string_t<Args...> message, Args &&...args)
		{
			s_ClientLogger->warn(message, std::forward<Args>(args)...);
		}
		template <typename... Args>
		static void Error(spdlog::format_string_t<Args...> message, Args &&...args)
		{
			s_ClientLogger->error(message, std::forward<Args>(args)...);
		}
		template <typename... Args>
		static void Critical(spdlog::format_string_t<Args...> message, Args &&...args)
		{
			s_ClientLogger->critical(message, std::forward<Args>(args)...);
		}

		static void Trace(const char* message);
		static void Info(const char* message);
		static void Warn(const char* message);
		static void Error(const char* message);
		static void Critical(const char* message);

	private:
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}