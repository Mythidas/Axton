#pragma once

#include "Component.h"
#include "Axton/Core/Defines.h"
#include "Axton/Math/Math.h"

namespace Axton
{
	class Tag : public ECSComponent
	{
		std::string Name;
	};

	class Transform : public ECSComponent
	{
		Vector3 Position{ 0.0f, 0.0f, 0.0f };
		Vector3 Scale{ 1.0f, 1.0f, 1.0f };
		Vector3 Rotation{ 0.0f, 0.0f, 0.0f };
	};
}