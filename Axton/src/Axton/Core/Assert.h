#pragma once

#ifdef AX_ASSERTS_ENABLED
	#define AX_ASSERT(x, ...)		{ if(!(x)) { Log::Error("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define AX_ASSERT_CORE(x, ...)	{ if(!(x)) { CoreLog::Error("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define AX_ASSERT(...)
	#define AX_ASSERT_CORE(...)
#endif
