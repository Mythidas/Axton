#pragma once

#include "Axton/Core/Defines.h"

namespace Axton
{
	struct Interval
	{
	public:
		float Min;
		float Max;

		Interval();
		Interval(float min, float max);
		Interval(const Interval& in1, const Interval& in2);

		bool Contains(float value) const;
		bool Surrounds(float value) const;
		float Clamp(float value) const;
		float Size() const;

		Interval Expand(float delta) const;

		static const Interval Empty;
		static const Interval Infinite;
	};
}