#pragma once

#include "Axton/Core/Layer.h"

namespace Axton
{
	class ImGUILayer : public Layer
	{
	public:
		virtual ~ImGUILayer() = default;

		virtual void BeginUI() const = 0;
		virtual void EndUI() const = 0;

		static ImGUILayer* Create();
	};
}