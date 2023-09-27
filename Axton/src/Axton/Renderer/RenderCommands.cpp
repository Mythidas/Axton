#include "axpch.h"
#include "RenderCommands.h"
#include "RendererAPI.h"

namespace Axton
{
	Scope<RendererAPI> RenderCommands::s_API = RendererAPI::Create();
}