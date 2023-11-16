#pragma once

#include <spdlog/spdlog.h>

#include <memory>

namespace spdlog
{
	class logger;
}

namespace Axton
{
	// Console logging
	class CoreLog
	{
	public:
		static void Construct();

#ifndef AX_DIST
		template <typename... Args>
		static void Trace(spdlog::format_string_t<Args...> message, Args &&...args)
		{
			s_CoreLogger->trace(message, std::forward<Args>(args)...);
		}
		template <typename... Args>
		static void Info(spdlog::format_string_t<Args...> message, Args &&...args)
		{
			s_CoreLogger->info(message, std::forward<Args>(args)...);
		}
		template <typename... Args>
		static void Warn(spdlog::format_string_t<Args...> message, Args &&...args)
		{
			s_CoreLogger->warn(message, std::forward<Args>(args)...);
		}
		template <typename... Args>
		static void Error(spdlog::format_string_t<Args...> message, Args &&...args)
		{
			s_CoreLogger->error(message, std::forward<Args>(args)...);
		}
		template <typename... Args>
		static void Critical(spdlog::format_string_t<Args...> message, Args &&...args)
		{
			s_CoreLogger->critical(message, std::forward<Args>(args)...);
		}
#else
		template <typename... Args>
		static void Trace(spdlog::format_string_t<Args...> message, Args &&...args) {}
		template <typename... Args>
		static void Info(spdlog::format_string_t<Args...> message, Args &&...args) {}
		template <typename... Args>
		static void Warn(spdlog::format_string_t<Args...> message, Args &&...args) {}
		template <typename... Args>
		static void Error(spdlog::format_string_t<Args...> message, Args &&...args) {}
		template <typename... Args>
		static void Critical(spdlog::format_string_t<Args...> message, Args &&...args) {}
#endif

		static void Trace(const char* message);
		static void Info(const char* message);
		static void Warn(const char* message);
		static void Error(const char* message);
		static void Critical(const char* message);

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
	};
}