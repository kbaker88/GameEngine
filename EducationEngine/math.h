#ifndef MATH_H
#define MATH_H

#include <cmath> // TODO : SIMD? remove this

#define Maximum(A, B) ((A > B) ? (A) : (B))
const float PI = 3.141592653589793f;

inline double FloorFloatToInt64(float Value)
{
	double Result;
	Result = (double)floor(Value);
	return Result;
}

inline int Floor32(int Value)
{
	return (int)floor(Value);
}

inline float Square(float A)
{
	float Result = A*A;

	return(Result);
}

inline float SquareRoot(float Value) 
{
	float Result = sqrtf(Value);
	return(Result);
}

inline float Sin(float Angle)
{
	float Result = sinf(Angle);
	return(Result);
}

inline float Cos(float Angle)
{
	float Result = cosf(Angle);
	return(Result);
}

inline int AbsVal(int Value)
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