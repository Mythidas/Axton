#include "axpch.h"
#include "Log.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Axton
{
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	void Log::Construct()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");

		s_ClientLogger = spdlog::stdout_color_mt("App");
		s_ClientLogger->set_level(spdlog::level::trace);
	}

	void Log::Trace(const char* message)
	{
		s_ClientLogger->trace(message);
	}

	void Log::Info(const char* message)
	{
		s_ClientLogger->info(message);
	}

	void Log::Warn(const char* message)
	{
		s_ClientLogger->warn(message);
	}

	void Log::Error(const char* message)
	{
		s_ClientLogger->error(message);
	}

	void Log::Critical(const char* message)
	{
		s_ClientLogger->critical(message);
	}
}