#include "math.h"

float Math_Floor(float Value)
{
	float Result = 0.0f;

	union
	{
		float BitsAsFloat;
		int   BitsAsInt;
	} ValueStorage;

	if (Value == 0.0f)
	{
		return 0.0f;
	}

	ValueStorage.BitsAsFloat = Value;

	int FloatingPointExponent = ValueStorage.BitsAsInt & (255 << 23);
	FloatingPointExponent = (FloatingPointExponent >> 23) - 127;

	int Significand = ValueStorage.BitsAsInt & ((1 << 23) - 1);

	if (FloatingPointExponent > 0)
	{
		Result = (float)((Significand >> (23 - FloatingPointExponent)) |
			(1 << FloatingPointExponent));
	}
	else
	{
		Result = 0.0f;
	}

	bool ValueIsNegative = (ValueStorage.BitsAsInt >> 31) != 0;

	if (ValueIsNegative)
	{
		if (Result != 0.0f)
		{
			Result = -Result;
		}
		Result--;
	}

	return Result;
}