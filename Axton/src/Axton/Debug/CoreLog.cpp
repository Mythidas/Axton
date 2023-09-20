#include "axpch.h"
#include "CoreLog.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Axton
{
	std::shared_ptr<spdlog::logger> CoreLog::s_CoreLogger;

	void CoreLog::Construct()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");

		s_CoreLogger = spdlog::stdout_color_mt("Axton");
		s_CoreLogger->set_level(spdlog::level::trace);
	}

#ifndef AX_DIST
	void CoreLog::Trace(const char* message)
	{
		s_CoreLogger->trace(message);
	}

	void CoreLog::Info(const char* message)
	{
		s_CoreLogger->info(message);
	}

	void CoreLog::Warn(const char* message)
	{
		s_CoreLogger->warn(message);
	}

	void CoreLog::Error(const char* message)
	{
		s_CoreLogger->error(message);
	}

	void CoreLog::Critical(const char* message)
	{
		s_CoreLogger->critical(message);
	}
#else
	void CoreLog::Trace(const char* message) {}
	void CoreLog::Info(const char* message) {}
	void CoreLog::Warn(const char* message) {}
	void CoreLog::Error(const char* message) {}
	void CoreLog::Critical(const char* message) {}
#endif
}