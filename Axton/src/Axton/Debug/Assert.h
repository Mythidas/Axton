#pragma once

#include "Log.h"
#include "CoreLog.h"
#include <spdlog/spdlog.h>

namespace Axton
{
	static void* Assert(bool assert, std::string message)
	{
		if (!assert)
		{
			Log::Error("Assertion Failed: {0}", message);
			__debugbreak();
		}

		return nullptr;
	}

	static void* AssertCore(bool assert, std::string message)
	{
		if (!assert)
		{
			CoreLog::Error("Assertion Failed: {0}", message);
			__debugbreak();
		}

		return nullptr;
	}
}