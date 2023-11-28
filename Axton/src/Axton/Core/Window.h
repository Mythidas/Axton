#pragma once

#include "Defines.h"
#include "Axton/Utils/Event.h"

#include <string>

namespace Axton
{
	class Window
	{
	public:
		struct Specs
		{
			Specs& setTitle(const std::string& title) { Title = title; return *this; }
			Specs& setWidth(const uint32_t width) { Width = width; return *this; }
			Specs& setHeight(const uint32_t height) { Height = height; return *this; }
			Specs& setFixedAspectRatio(const bool aspr) { FixedAspectRatio = aspr; return *this; }
			Specs& setVSync(const bool vsync) { VSync = vsync; return *this; }
			Scope<Window> Build() const { return Window::Create(*this); }

			std::string Title{ "AxtonEngine" };
			uint32_t Width{ 1280 };
			uint32_t Height{ 720 };
			bool FixedAspectRatio{ true };
			bool VSync{ false };
		};

		static Event<> OnWindowClose; // No arguments
		static Event<int, int> OnWindowResize; // Width, Height
		static Event<int> OnKeyPressed; // Key
		static Event<int> OnKeyReleased; // Key
		static Event<double, double> OnMouseMoved; // Mouse X, Mouse Y
		static Event<int> OnMouseButtonPressed; // Button
		static Event<int> OnMouseButtonReleased; // Button

		virtual ~Window() = default;

		virtual void OnUpdate() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void SetCursorMode(bool locked) = 0;

		virtual void* GetNativeWindow() = 0;

		static Scope<Window> Create(const Specs& props);
	};
}