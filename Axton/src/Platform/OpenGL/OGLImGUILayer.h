#pragma once

#include "Axton/ImGUI/ImGUILayer.h"

namespace Axton::OpenGL
{
	class OGLImGUILayer : public Axton::ImGUILayer
	{
	public:
		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void BeginUI() const override;
		virtual void EndUI() const override;
	};
}