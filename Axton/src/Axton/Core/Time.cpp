#include "axpch.h"
#include "Time.h"

namespace Axton
{
	const float Time::FixedDeltaTime = 0.02f;

	Time::TimePoint Time::m_LastFrame;
	Time::TimePoint Time::m_LastFixedFrame;
	float Time::m_DeltaTime;
	float Time::m_FixedDeltaTime;

	void Time::Construct()
	{
		m_LastFrame = std::chrono::high_resolution_clock::now();
	}

	void Time::OnUpdate()
	{
		TimePoint now = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> delta = now - m_LastFrame;
		m_LastFrame = now;
		m_DeltaTime = delta.count();

		std::chrono::duration<float> fixedDelta = now - m_LastFixedFrame;
		if (fixedDelta.count() >= FixedDeltaTime)
		{
			m_LastFixedFrame = now;
			m_FixedDeltaTime = fixedDelta.count();
		}
		else
		{
			m_FixedDeltaTime = 0.0f;
		}
	}
}