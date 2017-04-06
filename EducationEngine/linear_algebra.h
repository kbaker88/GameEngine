#ifndef LINEAR_ALGEBRA_H
#define LINEAR_ALGEBRA_H

#include "math.h"

/* TODO
	- Rotate 4x4 matrix
*/

// Data Types
union v2
{
	struct
	{
		float x, y;
	};
	float Arr[2];

	v2() : x(0.0f), y(0.0f) {}

	v2(float X, float Y)
	{
		x = X;
		y = Y;
	}
	
	v2 & operator=(v2 & B)
	{
		x = B.x;
		y = B.y;
		return *this;
	}

};

union v3
{
	struct
	{
		float x, y, z;
	};
	struct
	{
		float r, g, b;
	};
	float Arr[3];

	v3() : x(0.0f), y(0.0f), z(0.0f) {}

	v3(float X, float Y, float Z)
	{
		x = X;
		y = Y;
		z = Z;
	}

	v3 & operator=(v3& B)
	{
		x = B.x;
		y = B.y;
		z = B.z;
		return *this;
	}
};

union v4
{
	struct
	{
		float x, y, z, w;
	};
	struct
	{
		float r, g, b, a;
	};
	float Arr[4];

	v4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}

	v4(float X, float Y, float Z, float W)
	{
		x = X;
		y = Y;
		z = Z;
		w = W;
	}
};

union m4
{
	struct
	{
		float a, b, c, d;
		float e, f, g, h;
		float i, j, k, l;
		float m, n, o, p;
	};
	float Rc[4][4];
	float Arr[16];

	//TODO: Is there a better way to initialize to zero here?
	m4() 
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				Rc[i][j] = 0.0f;
			}
		}
		Rc[0][0] = 1.0f;
		Rc[1][1] = 1.0f;
		Rc[2][2] = 1.0f;
		Rc[3][3] = 1.0f;
	}

	m4 & operator=(m4& B)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				Rc[i][j] = B.Rc[i][j];
			}
		}
		return *this;
	}
};

// Vector Operations (+, -, +=, -=, *, *=, ==)
inline v2 operator+(v2 A, v2 B)
{
	v2 Result;

	Result.x = A.x + B.x;
	Result.y = A.y + B.y;

	return(Result);
}

inline v2 operator-(v2 A)
{
	v2 Result;

	Result.x = -A.x;
	Result.y = -A.y;

	return(Result);
}

inline v2 operator-(v2 A, v2 B)
{
	v2 Result;

	Result.x = A.x - B.x;
	Result.y = A.y - B.y;

	return(Result);
}

inline v2 & operator+=(v2 &A, v2 B)
{
	A = A + B;

	return(A);
}

inline v2 & operator-=(v2 &A, v2 B)
{
	A = A - B;

	return(A);
}

inline v2 operator*(float A, v2 B)
{
	v2 Result;

	Result.x = A*B.x;
	Result.y = A*B.y;

	return(Result);
}

inline v2 operator*(v2 B, float A)
{
	v2 Result = A * B;

	return(Result);
}

inline v2 & operator*=(v2 &B, float A)
{
	B = A * B;

	return(B);
}

inline bool operator==(v2 &A, v2 &B)
{
	if (A.x == B.x)
		if (A.y == B.y)
			return true;

	return false;
}

inline bool operator==(v2 &A, const v2 &B)
{
	if (A.x == B.x)
		if (A.y == B.y)
			return true;

	return false;
}

inline v3 operator+(v3 A, v3 B)
{
	v3 Result;

	Result.x = A.x + B.x;
	Result.y = A.y + B.y;
	Result.z = A.z + B.z;

	return(Result);
}

inline v3 operator-(v3 A)
{
	v3 Result;

	Result.x = -A.x;
	Result.y = -A.y;
	Result.z = -A.z;

	return(Result);
}

inline v3 operator-(v3 A, v3 B)
{
	v3 Result;

	Result.x = A.x - B.x;
	Result.y = A.y - B.y;
	Result.z = A.z - B.z;

	return(Result);
}

inline v3 & operator+=(v3 &A, v3 B)
{
	A = A + B;

	return(A);
}

inline v3 & operator-=(v3 &A, v3 B)
{
	A = A - B;

	return(A);
}

inline v3 operator*(float A, v3 B)
{
	v3 Result;

	Result.x = A*B.x;
	Result.y = A*B.y;
	Result.z = A*B.z;

	return(Result);
}

inline v3 operator*(v3 B, float A)
{
	v3 Result = A * B;

	return(Result);
}

inline v3 operator*(v3 A, v3 B)
{
	v3 Result;

	Result.x = A.x * B.x;
	Result.y = A.y * B.y;
	Result.z = A.z * B.z;

	return(Result);
}

inline v3 & operator*=(v3 &B, float A)
{
	B = A * B;

	return(B);
}

inline bool operator==(v3 &A, v3 &B)
{
	if (A.x == B.x)
		if (A.y == B.y)
			if (A.z == B.z)
				return true;

	return false;
}

inline bool operator==(v3 &A, const v3 &B)
{
	if (A.x == B.x)
		if (A.y == B.y)
			if (A.z == B.z)
				return true;

	return false;
}

inline v4 operator+(v4 A, v4 B)
{
	v4 Result;

	Result.x = A.x + B.x;
	Result.y = A.y + B.y;
	Result.z = A.z + B.z;
	Result.w = A.w + B.w;

	return(Result);
}

inline v4 operator-(v4 A, v4 B)
{
	v4 Result;

	Result.x = A.x - B.x;
	Result.y = A.y - B.y;
	Result.z = A.z - B.z;
	Result.w = A.w - B.w;

	return(Result);
}

inline v4 operator*(float A, v4 B)
{
	v4 Result;

	Result.x = A*B.x;
	Result.y = A*B.y;
	Result.z = A*B.z;
	Result.w = A*B.w;

	return(Result);
}

inline v4 operator*(v4 B, float A)
{
	v4 Result = A * B;

	return(Result);
}

inline v4 operator*(m4& Matrix, v4& Vec)
{
	v4 Result;

	Result.x = { (Matrix.Rc[0][0] * Vec.x) +
		(Matrix.Rc[0][1] * Vec.y) +
		(Matrix.Rc[0][2] * Vec.z) +
		(Matrix.Rc[0][3] * Vec.w) };

	Result.y = { (Matrix.Rc[1][0] * Vec.x) +
		(Matrix.Rc[1][1] * Vec.y) +
		(Matrix.Rc[1][2] * Vec.z) +
		(Matrix.Rc[1][3] * Vec.w) };

	Result.z = { (Matrix.Rc[2][0] * Vec.x) +
		(Matrix.Rc[2][1] * Vec.y) +
		(Matrix.Rc[2][2] * Vec.z) +
		(Matrix.Rc[2][3] * Vec.w) };

	Result.w = { (Matrix.Rc[3][0] * Vec.x) +
		(Matrix.Rc[3][1] * Vec.y) +
		(Matrix.Rc[3][2] * Vec.z) +
		(Matrix.Rc[3][3] * Vec.w) };

	return(Result);
}

// Matrix Operations
inline v3 operator*(m4& Matrix, v3& Vec) //TODO: Get Rid of this, can cause bad results if people don't know implementation.
{
	v3 Result;

	Result.x = { (Matrix.Rc[0][0] * Vec.x) +
		(Matrix.Rc[0][1] * Vec.y) +
		(Matrix.Rc[0][2] * Vec.z) +
		(Matrix.Rc[0][3] * 0.0f) };

	Result.y = { (Matrix.Rc[1][0] * Vec.x) +
		(Matrix.Rc[1][1] * Vec.y) +
		(Matrix.Rc[1][2] * Vec.z) +
		(Matrix.Rc[1][3] * 0.0f) };

	Result.z = { (Matrix.Rc[2][0] * Vec.x) +
		(Matrix.Rc[2][1] * Vec.y) +
		(Matrix.Rc[2][2] * Vec.z) +
		(Matrix.Rc[2][3] * 0.0f) };

	return(Result);
}

inline v3 operator*(v3 Vec, m4 Matrix)
{
	return(Matrix * Vec);
}

m4 operator*(m4 &A, m4 &B);

// Vector Math
inline v2 Math_Orthogonal(v2 A)
{
	v2 Result = { -A.y, A.x };
	return(Result);
}

inline float Math_InnerProduct(v3* A, v3* B)
{
	float Result = A->x*B->x + A->y*B->y + A->z*B->z;

	return(Result);
}

inline float Math_Length(v3 A)
{
	float Result = Math_InnerProduct(&A, &A);
	Result = Math_SquareRoot(Result);
	return(Result);
}


inline v3 Math_Normalize(v3 A)
{
	v3 Result = A * (1.0f / Math_Length(A));

	return(Result);
}

inline float Math_Magnitude(v3 &A)
{
	return Math_Square((A.x * A.x + A.y * A.y + A.z * A.z));
}

v3 Math_CrossProduct(v3 &A, v3 &B);
v3 Math_ReflectVector(v3 &IncomingVect, v3 &Normal);

// Matrix Math
m4 Math_IdentityMatrix();
m4 Math_LookAtMatrix(v3 &Eye, v3 &Target, v3 &Up);
m4 Math_PerspectiveMatrix(float FieldOfView, float AspectRatio, 
	float NearPlane, float FarPlane);
m4 Math_OrthographicMarix(float Left, float Right, float Bottom,
	float Top, float Near, float Far);
m4 Math_TranslateMatrix(m4 &Matrix, v3 &Vector);
m4 Math_ScaleMatrix(m4 &Matrix, v3 &Vector);
m4 Math_InvertMatrix(m4 *Matrix);
//m4 MatrixInverse(m4 &Matrix); // expensive
//float Determinant(m4 &Mat); // expensive

#endif