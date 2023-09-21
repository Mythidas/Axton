#include "axpch.h"
#include "Events.h"

namespace Axton
{
	Event<> Events::OnWindowClose;
	Event<int, int>	Events::OnWindowResize;
}