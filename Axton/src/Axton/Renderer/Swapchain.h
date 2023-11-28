#pragma once

#include "Axton/Core/Defines.h"
#include "RenderPass.h"
#include "Axton/Math/Extent.h"

namespace Axton
{
	class Swapchain
	{
	public:
		struct Specs
		{
			Ref<RenderPass> pRenderPass;

			Specs& setRenderPass(Ref<RenderPass> renderPass) { pRenderPass = renderPass; return *this; }
			Ref<Swapchain> Build() { return Create(*this); }
		};

		virtual void Recreate() = 0;

		virtual Extent2D GetExtent() const = 0;
		virtual Ref<RenderPass> GetRenderPass() const = 0;

		static Ref<Swapchain> Create(const Specs& specs);
	};
}