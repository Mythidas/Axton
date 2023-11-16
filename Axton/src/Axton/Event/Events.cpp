#include "axpch.h"
#include "Events.h"

namespace Axton
{
	Event<> Events::OnWindowClose;
	Event<int, int>	Events::OnWindowResize;
	Event<int> Events::OnKeyPressed;
	Event<int> Events::OnKeyReleased;
	Event<double, double> Events::OnMouseMoved;
	Event<int> Events::OnMouseButtonPressed;
	Event<int> Events::OnMouseButtonReleased;
}