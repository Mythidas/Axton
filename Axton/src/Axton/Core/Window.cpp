#include "axpch.h"
#include "Window.h"

#include "Platform/Windows/WindowsWindow.h"

namespace Axton
{
	Scope<Window> Window::Create(const Specs& props)
	{
		// TODO: Detect platform and return the correct window
		return CreateScope<WindowsWindow>(props);
	}
}