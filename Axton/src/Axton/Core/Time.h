#pragma once

#include "Defines.h"

#include <chrono>

namespace Axton
{
	class AX_API Time
	{
		typedef std::chrono::steady_clock::time_point TimePoint;
	public:
		static const float FixedDeltaTime;

	public:
		static void Construct();
		static void OnUpdate();

		static const float GetDeltaTime() { return m_DeltaTime; }
		static const float GetFixedDeltaTime() { return m_FixedDeltaTime; }

	private:
		static TimePoint m_LastFrame;
		static TimePoint m_LastFixedFrame;

		static float m_DeltaTime;
		static float m_FixedDeltaTime;
	};
}