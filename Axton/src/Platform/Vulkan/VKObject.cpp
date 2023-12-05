#include "axpch.h"
#include "VKObject.h"
#include "VKRenderEngine.h"

namespace Axton::Vulkan
{
	VKObject::VKObject()
	{
		VKRenderEngine::RegisterObject(this);
	}

	VKObject::~VKObject()
	{
		VKRenderEngine::UnregisterObject(this);
	}
}