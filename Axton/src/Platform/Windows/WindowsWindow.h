#pragma once

#include "Axton/Core/Window.h"

#include <GLFW/glfw3.h>

namespace Axton
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const Window::Specs& specs);
		virtual ~WindowsWindow() override;

		virtual void OnUpdate() override;

		virtual uint32_t GetWidth() const override { return m_Specs.Width; }
		virtual uint32_t GetHeight() const override { return m_Specs.Height; }

		virtual void SetCursorMode(bool locked) override;

		virtual void* GetNativeWindow() override { return m_Window; }

	private:
		GLFWwindow* m_Window;
		Window::Specs m_Specs;
	};
}