#pragma once

#include "Defines.h"
#include "Axton/Renderer/GraphicsContext.h"

#include <string>

namespace Axton
{
	struct WindowSpecs
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;
		bool FixedAspectRatio = true;

		WindowSpecs(const std::string& title = "Axton Engine",
			unsigned int width = 1280,
			unsigned int height = 720)
			: Title(title), Width(width), Height(height)
		{
		}
	};

	class Window
	{
	public:
		virtual ~Window() = default;

		virtual void OnUpdate() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void SetCursorMode(bool locked) = 0;

		virtual void* GetNativeWindow() const = 0;
		virtual GraphicsContext& GetContext() const = 0;

		static Scope<Window> Create(const WindowSpecs& props = WindowSpecs());
	};
}