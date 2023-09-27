#pragma once

#include "Axton/Core/Defines.h"

#include <chrono>

namespace Axton
{
	class AX_API Timer
	{
	public:
		Timer(const std::string& name);
		~Timer();

	private:
		std::string m_Name;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
	};
}