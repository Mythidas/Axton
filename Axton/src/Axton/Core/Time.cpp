#include "axpch.h"
#include "Time.h"

namespace Axton
{
	const float Time::FixedDeltaTime = 0.02f;

	Time::TimePoint Time::s_LastFrame;
	Time::TimePoint Time::s_LastFixedFrame;

	std::vector<float> Time::s_FrameTimes;

	float Time::s_AverageFPS;
	float Time::s_DeltaTime;
	float Time::s_FixedDeltaTime;

	static float s_OneSecond = 1.0f;

	void Time::Construct()
	{
		s_LastFrame = std::chrono::high_resolution_clock::now();
	}

	void Time::OnUpdate()
	{
		TimePoint now = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> delta = now - s_LastFrame;
		s_LastFrame = now;
		s_DeltaTime = delta.count();

		s_OneSecond -= s_DeltaTime;
		if (s_OneSecond < 0.0f)
		{
			float total = 0.0f;
			for (auto& ds : s_FrameTimes)
				total += ds;

			s_AverageFPS = 1.0f / (total / s_FrameTimes.size());
			s_FrameTimes.clear();
			s_OneSecond = 1.0f;
		}
		else
		{
			s_FrameTimes.push_back(s_DeltaTime);
		}

		std::chrono::duration<float> fixedDelta = now - s_LastFixedFrame;
		if (fixedDelta.count() >= FixedDeltaTime)
		{
			s_LastFixedFrame = now;
			s_FixedDeltaTime = fixedDelta.count();
		}
		else
		{
			s_FixedDeltaTime = 0.0f;
		}
	}
}