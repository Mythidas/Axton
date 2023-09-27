#include "axpch.h"
#include "Ray.h"

namespace Axton
{
	Ray::Ray(const Ray& ray)
		: Origin(ray.Origin), Direction(ray.Direction)
	{
	}

	Ray::Ray(const Vector3& origin, const Vector3& direction)
		: Origin(origin), Direction(direction)
	{
	}

	Vector3 Ray::GetPoint(float distance) const
	{
		return Origin + Direction * distance;
	}
}