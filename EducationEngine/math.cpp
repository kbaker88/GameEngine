#include "math.h"

//NOTE: Math_Floor() is only for 32-bit floats on Little-Endian Machines
float 
Math_Floor(float Value)
{
	 //Shortest Method
	return Value - ((int)Value % 1);
	
	/* Short Method
	int Result = (int)Value;
	if (Value < 0)
	{
		Result -= 1;
	}
	return Result;
	*/

	/* Long Method
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
	*/
}

float
Math_Power(float Base, int Exponent)
{
	float Result = 1.0f;
	while (Exponent)
	{
		if (Exponent & 1)
			Result *= Base;
		Exponent >>= 1;
		Base *= Base;
	}

	return Result;
}

int
Math_Power(int Base, int Exponent)
{
	int Result = 1;
	while (Exponent)
	{
		if (Exponent & 1)
			Result *= Base;
		Exponent >>= 1;
		Base *= Base;
	}

	return Result;
}
//NOTE: Uses Newton's Method (NewGuess = OldGuess - f(OldGuess) / f'(OldGuess)) 
//      with f(Guess) = Guess^2 - Radicand
//TODO: Look for performance improvements
float
Math_SquareRoot(float Value)
{
	if (Value == 1.0f)
	{
		return 1.0f;
	}
	else if (Value == 0.0f)
	{
		return 0.0f;
	}
	else if (Value < 0.0f)
	{
		return -1.0f; // Error
	}

	float Result = Value * 0.5f; //TODO: Find a better initial guess.
	float HalfValue = Result;
	float Precision = 0.0000001f;

	for (unsigned int i = 0; i < 100; i++)
	{
		Result -= (Result * 0.5f) - (HalfValue / Result);
		if (Math_AbsVal(Result) <= Precision)
			break;
	}

	return Result;
}

// NOTE: Uses Newton's Method (NewGuess = OldGuess - f(OldGuess) / f'(OldGuess))
//      f(Guess) = Guess^Degree - Radicand
// TODO: Reduce decision spots
float
Math_Root(float Value, unsigned int Degree)
{
	if (Value == 1.0f)
	{
		return 1.0f;
	}
	else if (Value == 0.0f)
	{
		return 0.0f;
	}
	else if (Value < 0.0f)
	{
		return -1.0f; // Error
	}

	if (Degree == 1)
	{
		return Value;
	}
	else if (Degree < 1)
	{
		return -1.0f; // Error
	}

	float Result = Value * 0.5f; //TODO: Find a better initial guess.
	float Precision = 0.0000001f;

	float DegreeReciprocal = 1.0f / static_cast<float>(Degree);
	float RadicandMulInverseDegree = Value * DegreeReciprocal;

	for (unsigned int i = 0; i < 100; i++)
	{
		Result -= (Result * DegreeReciprocal) -
			(RadicandMulInverseDegree / Math_Power(Result, Degree - 1));
		if (Math_AbsVal(Result) <= Precision)
			break;
	}

	return Result;
}

int
Math_Factorial(int Value)
{
	int Result = Value;

	for (int i = 1; i < Value; i++)
	{
		Result *= (Value - i);
	}

	return Result;
}

float 
Math_Sine(float Radians)
{
	while (Radians < -PI)
	{
		Radians += 2.0f * PI;
	}
	
	while (Radians > PI)
	{
		Radians -= 2.0f * PI;
	}
	
	double Result = Radians;
	bool FlipSign = false;
	
	//TODO: Pre-compute the factorials on startup.
	for (unsigned int i = 3; i < 11; i = i + 2)
	{
		if (FlipSign)
		{
			Result += Math_Power(Radians, i) / (double)Math_Factorial(i);
			FlipSign = 0;
		}
		else
		{
			Result -= Math_Power(Radians, i) / (double)Math_Factorial(i);
			FlipSign = 1;
		}
	}
	return (float)Result;
	//return sinf(Radians);
}

float
Math_Cosine(float Radians)
{
	while (Radians < -PI)
	{
		Radians += 2.0f * PI;
	}
	
	while (Radians > PI)
	{
		Radians -= 2.0f * PI;
	}
	
	double Result = 1;
	bool FlipSign = false;
	
	//TODO: Pre-compute the factorials on startup.
	for (unsigned int i = 2; i < 11; i = i + 2)
	{
		if (FlipSign)
		{
			Result += Math_Power(Radians, i) / (double)Math_Factorial(i);
			FlipSign = 0;
		}
		else
		{
			Result -= Math_Power(Radians, i) / (double)Math_Factorial(i);
			FlipSign = 1;
		}
	}
	return (float)Result;
	//return cosf(Radians);
}

float 
Math_Tangent(float Radians)
{
	return (Math_Sine(Radians) / Math_Cosine(Radians));
	//return tanf(Radians);
}