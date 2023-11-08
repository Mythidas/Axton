#pragma once

#include "Axton/ImGUI/ImGUILayer.h"

#include <vulkan/vulkan.hpp>

namespace Axton::Vulkan
{
	class VKImGUILayer : public ImGUILayer
	{
	public:
		VKImGUILayer();

		virtual void BeginUI() const override;
		virtual void EndUI() const override;
	};
}