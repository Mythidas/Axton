#include "axpch.h"
#include "Interval.h"
#include "Math.h"

namespace Axton
{
	const Interval Interval::Empty = Interval(Mathf::Infinity::Float, -Mathf::Infinity::Float);
	const Interval Interval::Infinite = Interval(-Mathf::Infinity::Float, Mathf::Infinity::Float);

	Interval::Interval()
	{
		*this = Empty;
	}

	Interval::Interval(float min, float max)
		: Min(min), Max(max)
	{
	}

	Interval::Interval(const Interval& in1, const Interval& in2)
		: Min(fmin(in1.Min, in2.Min)), Max(fmax(in1.Max, in2.Max))
	{
	}

	bool Interval::Contains(float value) const
	{
		return Min <= value && value <= Max;
	}

	bool Interval::Surrounds(float value) const
	{
		return Min < value && value < Max;
	}

	float Interval::Clamp(float value) const
	{
		return Mathf::Clamp::Float(value, Min, Max);
	}

	float Interval::Size() const
	{
		return Max - Min;
	}

	Interval Interval::Expand(float delta) const
	{
		float padding = delta / 2.0f;
		return Interval(Min - padding, Max + padding);
	}
}