#pragma once

#include "Axton/Core/Defines.h"
#include "Vector.h"

namespace Axton
{
	struct AX_API Ray
	{
		Ray() = default;
		Ray(const Ray& ray);
		Ray(const Vector3& origin, const Vector3& direction);

		Vector3 GetPoint(float distance) const;

		Vector3 Origin;
		Vector3 Direction;
	};
}