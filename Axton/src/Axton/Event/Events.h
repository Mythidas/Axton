#pragma once

#include "Event.h"
#include "Axton/Core/Defines.h"

namespace Axton
{
	class Events
	{
	public:
		static Event<> OnWindowClose; // No arguments
		static Event<int, int> OnWindowResize; // Width, Height
		static Event<int> OnKeyPressed; // Key
		static Event<int> OnKeyReleased; // Key
		static Event<double, double> OnMouseMoved; // Mouse X, Mouse Y
		static Event<int> OnMouseButtonPressed; // Button
		static Event<int> OnMouseButtonReleased; // Button
	};
}