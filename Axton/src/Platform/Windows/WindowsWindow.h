#pragma once

#include "Axton/Renderer/GraphicsContext.h"
#include "Axton/Core/Window.h"

#include <GLFW/glfw3.h>

namespace Axton
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowSpecs& specs);
		virtual ~WindowsWindow() override;

		virtual void OnUpdate() override;

		virtual uint32_t GetWidth() const override { return m_Specs.Width; }
		virtual uint32_t GetHeight() const override { return m_Specs.Height; }

		virtual void SetCursorMode(bool locked) override;

		virtual void* GetNativeWindow() const override { return m_Window; }
		virtual GraphicsContext& GetContext() const override { return *m_Context; }

	private:
		GLFWwindow* m_Window;
		WindowSpecs m_Specs;
		Scope<GraphicsContext> m_Context;
	};
}