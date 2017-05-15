#ifndef MATH_H
#define MATH_H

//#include <cmath>
//TODO: Add lookup tables later

#define Maximum(A, B) ((A > B) ? (A) : (B))
#define PI 3.141592653589793f

// Conversions
inline float
Math_ConvertToRadians(float Degrees)
{
	return Degrees * 0.01745329251994329576923690768489f;
}

inline float
Math_ConvertToDegrees(float Radians)
{
	return Radians * 57.2958f;
}

inline int
Math_AbsVal(int Value)
{
	if (Value < 0)
	{
		return -Value;
	}
	else
	{
		return Value;
	}
}

inline long long 
Math_AbsVal(long long Value)
{
	if (Value < 0)
	{
		return -Value;
	}
	else
	{
		return Value;
	}
}

inline float
Math_AbsVal(float Value)
{
	if (Value < 0.0f)
	{
		return -Value;
	}
	else
	{
		return Value;
	}
}

inline float
Math_Square(float A)
{
	return(A*A);
}

inline int
Math_Square(int A)
{
	return(A*A);
}

//TODO: Create a Math_Floor for 64-bit values.
float
Math_Floor(float Value);
float
Math_Power(float Base, int Exponent);
int 
Math_Power(int Base, int Exponent);

float
Math_SquareRoot(float Value);
float
Math_Root(float Value, unsigned int Degree);
int
Math_Factorial(int Value);

// Trigonometry
float 
Math_Sine(float Radians);
float
Math_Cosine(float Radians);
float
Math_Tangent(float Radians);

// Linear Blend
// P1 + t(P2 - P1)

#endif