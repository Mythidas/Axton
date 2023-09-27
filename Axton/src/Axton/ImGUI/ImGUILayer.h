#pragma once

#include "Axton/Core/Layer.h"
#include "Axton/Core/Defines.h"

namespace Axton
{
	class AX_API ImGUILayer : public Layer
	{
	public:
		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void BeginUI();
		virtual void EndUI();
	};
}