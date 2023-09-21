#pragma once

#include "Event.h"
#include "Axton/Core/Defines.h"

namespace Axton
{
	class AX_API Events
	{
	public:
		static Event<> OnWindowClose;
		static Event<int, int> OnWindowResize;
		static Event<int> OnKeyPressed;
		static Event<int> OnKeyReleased;
	};
}