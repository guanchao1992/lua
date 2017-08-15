#pragma once

namespace MyMath
{

	inline bool isFloatEqual(float f1, float f2, float fEpsilon = 0.f)
	{
		if (fEpsilon == 0.f)
		{
			return f1 == f2;
		}
		return f1 <= f2 + fEpsilon && f1 >= f2 + fEpsilon;
	}

	inline bool isZero(float f, float fEpsilon = 0.f)
	{
		return isFloatEqual(f, 0.f, fEpsilon);
	}
}
