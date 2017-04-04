#ifndef MATH_H
#define MATH_H

#include <cmath> // TODO : SIMD? remove this

#define Maximum(A, B) ((A > B) ? (A) : (B))
const float PI = 3.141592653589793f;

// Conversions
inline float Math_ConverToRadians(float Degrees)
{
	return Degrees * 0.01745329251994329576923690768489f;
}

inline float Math_ConvertToDegrees(float Radians)
{
	return Radians * 57.2958f;
}

inline int Math_AbsVal(int Value)
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

float Math_Floor(float Value);

inline float Math_Square(float A)
{
	float Result = A*A;

	return(Result);
}

inline int Math_Square(int A)
{
	int Result = A*A;

	return(Result);
}

inline float Math_SquareRoot(float Value) 
{
	float Result = sqrtf(Value);
	return(Result);
}

// Trigonometry
inline float Math_Sin(float Angle)
{
	float Result = sinf(Angle);
	return(Result);
}

inline float Math_Cos(float Angle)
{
	float Result = cosf(Angle);
	return(Result);
}

// Linear Blend
// P1 + t(P2 - P1)

/* SQRT EXAMPLE FOR LATER

double sqrt(double x) {
if (x <= 0)
return 0;       // if negative number throw an exception?
int exp = 0;
x = frexp(x, &exp); // extract binary exponent from x
if (exp & 1) {      // we want exponent to be even
exp--;
x *= 2;
}
double y = (1+x)/2; // first approximation
double z = 0;
while (y != z) {    // yes, we CAN compare doubles here!
z = y;
y = (y + x/y) / 2;
}
return ldexp(y, exp/2); // multiply answer by 2^(exp/2)
}

*/

#endif