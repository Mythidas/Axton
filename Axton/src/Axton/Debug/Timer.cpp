#include "axpch.h"
#include "Timer.h"

namespace Axton
{
	Timer::Timer(const std::string& name)
	{
		m_Name = name;
		m_StartTimepoint = std::chrono::high_resolution_clock::now();
	}

	Timer::~Timer()
	{
		std::chrono::duration<float> elapsed = std::chrono::high_resolution_clock::now() - m_StartTimepoint;
		CoreLog::Info("{0} took {1}ms", m_Name, elapsed.count() * 1000);
	}
}