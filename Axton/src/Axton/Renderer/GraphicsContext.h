#pragma once

#include "Axton/Core/Defines.h"

namespace Axton
{
	class GraphicsContext
	{
	public:
		virtual ~GraphicsContext() = default;

		virtual void Init(void* window) = 0;
		virtual void SwapBuffers() = 0;

		static Scope<GraphicsContext> Create();
	};
}