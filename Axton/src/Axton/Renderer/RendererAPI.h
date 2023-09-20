#pragma once

#include "Axton/Core/Defines.h"

namespace Axton
{
	typedef unsigned int RendererID;

	class AX_API RendererAPI
	{
	public:
		enum class API
		{
			None = 0,
			OpenGL = 1,
		};

	public:

		static API GetAPI() { return s_API; }
	private:
		static API s_API;
	};
}