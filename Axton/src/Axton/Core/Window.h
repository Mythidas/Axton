#pragma once

#include "Defines.h"
#include "Axton/Renderer/GraphicsContext.h"

#include <string>

namespace Axton
{
	class Window
	{
	public:
		struct Specs
		{
			std::string Title{ "AxtonEngine" };
			uint32_t Width{ 1280 };
			uint32_t Height{ 720 };
			bool FixedAspectRatio{ true };
			bool VSync{ false };
		};

		struct Builder
		{
			Builder& Title(const std::string& title) { m_Specs.Title = title; return *this; }
			Builder& Width(const uint32_t width) { m_Specs.Width = width; return *this; }
			Builder& Height(const uint32_t height) { m_Specs.Height = height; return *this; }
			Builder& FixedAspectRatio(const bool aspr) { m_Specs.FixedAspectRatio = aspr; return *this; }
			Builder& VSync(const bool vsync) { m_Specs.VSync = vsync; return *this; }
			Scope<Window> Build() const { return Window::Create(m_Specs); }

		private:
			Specs m_Specs;
		};

	public:
		virtual ~Window() = default;

		virtual void OnUpdate() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void SetCursorMode(bool locked) = 0;

		virtual void* GetNativeWindow() const = 0;
		virtual GraphicsContext& GetContext() const = 0;

		static Scope<Window> Create(const Specs& props);
	};
}