#pragma once

#include "Defines.h"

#include <chrono>

namespace Axton
{
	class Time
	{
		typedef std::chrono::steady_clock::time_point TimePoint;
	public:
		static const float FixedDeltaTime;

	public:
		static void Construct();
		static void OnUpdate();

		static const float GetFPS() { return s_AverageFPS; }
		static const float GetMSPerFrame() { return 1.0f / s_AverageFPS * 1000.0f; }
		static const float GetDeltaTime() { return s_DeltaTime; }
		static const float GetFixedDeltaTime() { return s_FixedDeltaTime; }

	private:
		static TimePoint s_LastFrame;
		static TimePoint s_LastFixedFrame;
		static std::vector<float> s_FrameTimes;

		static float s_AverageFPS;
		static float s_DeltaTime;
		static float s_FixedDeltaTime;
	};
}