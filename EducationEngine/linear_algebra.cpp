#include "linear_algebra.h"

m4 operator*(m4 &A, m4 &B)
{
	m4 Result;

	float Hold = 0;
	int j = 0;

	for (int k = 0; k < 4; k++)
	{
		for (int m = 0; m < 4; m++)
		{
			for (int i = 0; i < 4; i++)
			{
				Hold += A.Rc[k][j] * B.Rc[i][m];

				j++;
			}
			Result.Rc[k][m] = Hold;
			j = 0;
			Hold = 0;
		}
	}

	return Result;
}

v3 CrossProduct(v3 &A, v3 &B)
{
	v3 Result = {};
	Result.x = (A.y * B.z) - (A.z * B.y);
	Result.y = (A.z * B.x) - (A.x * B.z);
	Result.z = (A.x * B.y) - (A.y * B.x);

	return Result;
}

v3 ReflectVector(v3 &IncomingVect, v3 &Normal)
{
	v3 Result;
	Result = IncomingVect - 2.0f * InnerProduct(IncomingVect, Normal) * Normal;

	return Result;
}


m4 IdentityMatrix()
{
	m4 Result;
	
	Result.Rc[0][0] = 1.0f;
	Result.Rc[1][1] = 1.0f;
	Result.Rc[2][2] = 1.0f;
	Result.Rc[3][3] = 1.0f;

	return Result;
}

m4 LookAtMatrix(v3 &Eye, v3 &Center, v3 &Up)
{
	v3 f = Normalize(Center - Eye);
	v3 s = Normalize(CrossProduct(f, Up));
	v3 u = CrossProduct(s, f);

	m4 Result;
	Result.Rc[0][0] = s.x;
	Result.Rc[1][0] = s.y;
	Result.Rc[2][0] = s.z;
	Result.Rc[0][1] = u.x;
	Result.Rc[1][1] = u.y;
	Result.Rc[2][1] = u.z;
	Result.Rc[0][2] = -f.x;
	Result.Rc[1][2] = -f.y;
	Result.Rc[2][2] = -f.z;
	Result.Rc[3][0] = -InnerProduct(s, Eye);
	Result.Rc[3][1] = -InnerProduct(u, Eye);
	Result.Rc[3][2] = InnerProduct(f, Eye);
	return Result;
}

m4 PerspectiveMatrix(float FieldOfView, float AspectRatio, float NearPlane, float FarPlane)
{
	m4 Matrix;
	float HalfVal = (float)tan(FieldOfView / 2.0f);

	Matrix.Arr[0] = 1.0f / (AspectRatio * HalfVal);
	Matrix.Arr[1] = 0;
	Matrix.Arr[2] = 0;
	Matrix.Arr[3] = 0;

	Matrix.Arr[4] = 0;
	Matrix.Arr[5] = 1.0f / HalfVal;
	Matrix.Arr[6] = 0;
	Matrix.Arr[7] = 0;

	Matrix.Arr[8] = 0;
	Matrix.Arr[9] = 0;
	Matrix.Arr[10] = -(FarPlane + NearPlane) / (FarPlane - NearPlane);
	Matrix.Arr[11] = -1.0f;

	Matrix.Arr[12] = 0;
	Matrix.Arr[13] = 0;
	Matrix.Arr[14] = -(2.0f * FarPlane * NearPlane) / (FarPlane - NearPlane);
	Matrix.Arr[15] = 0;

	return Matrix;
}

m4 OrthographicMarix(float Left, float Right, float Bottom, float Top,
	float Near, float Far)
{
	m4 Matrix;

	Matrix.Rc[0][0] = 2.0f / (Right - Left);
	Matrix.Rc[1][1] = 2.0f / (Top - Bottom);
	Matrix.Rc[2][2] = -2.0f / (Far - Near);
	Matrix.Rc[3][0] = -(Right + Left) / (Right - Left);
	Matrix.Rc[3][1] = -(Top + Bottom) / (Top - Bottom);
	Matrix.Rc[3][2] = -(Far + Near) / (Far - Near);

	return Matrix;
}

m4 TranslateMatrix(m4 &Matrix, v3 &Vector)
{
	m4 Result = Matrix;

	Result.Rc[3][0] += Vector.Arr[0];
	Result.Rc[3][1] += Vector.Arr[1];
	Result.Rc[3][2] += Vector.Arr[2];

	return Result;
}

m4 ScaleMatrix(m4 &Matrix, v3 &Vector)
{
	m4 Result;

	Result.Rc[0][0] = Matrix.Rc[0][0] * Vector.Arr[0];
	Result.Rc[1][1] = Matrix.Rc[1][1] * Vector.Arr[1];
	Result.Rc[2][2] = Matrix.Rc[2][2] * Vector.Arr[2];
	Result.Rc[3][3] = Matrix.Rc[3][3];
	return Result;
}

m4 GetMatrixInverse(m4 *Matrix)
{
	m4 A = *Matrix;
	float Determinant = 1 / (A.Rc[0][0] * A.Rc[1][1] * A.Rc[2][2] + A.Rc[1][0] * A.Rc[2][1] * A.Rc[0][2]
		+ A.Rc[0][1] * A.Rc[1][2] * A.Rc[2][0] - A.Rc[2][0] * A.Rc[1][1] * A.Rc[0][2]
		- A.Rc[0][0] * A.Rc[2][1] * A.Rc[1][2] - A.Rc[1][0] * A.Rc[0][1] * A.Rc[2][2]);
	
	m4 Result;
	Result.Rc[0][0] = Determinant *(A.Rc[1][1] * A.Rc[2][2] * A.Rc[3][3] - A.Rc[2][1] * A.Rc[1][2] * A.Rc[3][3]);
	Result.Rc[0][1] = -1 * Determinant *(A.Rc[0][1] * A.Rc[2][2] * A.Rc[3][3] - A.Rc[2][1] * A.Rc[0][2] * A.Rc[3][3]);
	Result.Rc[0][2] = Determinant *(A.Rc[0][1] * A.Rc[1][2] * A.Rc[3][3] - A.Rc[1][1] * A.Rc[0][2] * A.Rc[3][3]);
	Result.Rc[0][3] = 0;
	Result.Rc[1][0] = -1 * Determinant * (A.Rc[1][0] * A.Rc[2][2] * A.Rc[3][3] - A.Rc[2][0] * A.Rc[1][2] * A.Rc[3][3]);
	Result.Rc[1][1] = Determinant * (A.Rc[0][0] * A.Rc[2][2] * A.Rc[3][3] - A.Rc[2][0] * A.Rc[0][2] * A.Rc[3][3]);
	Result.Rc[1][2] = -1 * Determinant * (A.Rc[0][0] * A.Rc[1][2] * A.Rc[3][3] - A.Rc[1][0] * A.Rc[0][2] * A.Rc[3][3]);
	Result.Rc[1][3] = 0;
	Result.Rc[2][0] = Determinant * (A.Rc[1][0] * A.Rc[2][1] * A.Rc[3][3] - A.Rc[1][1] * A.Rc[2][0] * A.Rc[3][3]);
	Result.Rc[2][1] = -1 * Determinant * (A.Rc[0][0] * A.Rc[2][1] * A.Rc[3][3] - A.Rc[2][0] * A.Rc[0][1] * A.Rc[3][3]);
	Result.Rc[2][2] = Determinant * (A.Rc[0][0] * A.Rc[1][1] * A.Rc[3][3] - A.Rc[1][0] * A.Rc[0][1] * A.Rc[3][3]);
	Result.Rc[2][3] = 0;
	Result.Rc[3][0] = -1 * Determinant * (A.Rc[1][0] * A.Rc[2][1] * A.Rc[3][2] + A.Rc[2][0] * A.Rc[3][1] * A.Rc[1][2]
		+ A.Rc[3][0] * A.Rc[1][1] * A.Rc[2][2] - A.Rc[1][0] * A.Rc[3][1] * A.Rc[2][2] - A.Rc[2][0] * A.Rc[1][1] * A.Rc[3][2]
		- A.Rc[3][0] * A.Rc[2][1] * A.Rc[1][2]);
	Result.Rc[3][1] = Determinant * (A.Rc[0][0] * A.Rc[2][1] * A.Rc[3][2] + A.Rc[2][0] * A.Rc[3][1] * A.Rc[0][2]
		+ A.Rc[3][0] * A.Rc[0][1] * A.Rc[2][2] - A.Rc[0][0] * A.Rc[3][1] * A.Rc[2][2] - A.Rc[2][0] * A.Rc[0][1] * A.Rc[3][2]
		- A.Rc[3][0] * A.Rc[2][1] * A.Rc[0][2]);
	Result.Rc[3][2] = -1 * Determinant * (A.Rc[0][0] * A.Rc[1][1] * A.Rc[3][2] + A.Rc[1][0] * A.Rc[3][1] * A.Rc[0][2]
		+ A.Rc[3][0] * A.Rc[0][1] * A.Rc[1][2] - A.Rc[0][0] * A.Rc[3][1] * A.Rc[1][2] - A.Rc[1][0] * A.Rc[0][1] * A.Rc[3][2]
		- A.Rc[3][0] * A.Rc[1][1] * A.Rc[0][2]);
	Result.Rc[3][3] = Determinant * (A.Rc[0][0] * A.Rc[1][1] * A.Rc[2][2] + A.Rc[1][0] * A.Rc[2][1] * A.Rc[0][2]
		+ A.Rc[2][0] * A.Rc[0][1] * A.Rc[1][2] - A.Rc[0][0] * A.Rc[2][1] * A.Rc[1][2] - A.Rc[1][0] * A.Rc[0][1] * A.Rc[2][2]
		- A.Rc[2][0] * A.Rc[1][1] * A.Rc[0][2]);

	return Result;
}

float Determinant(m4 &Mat)
{
	m4 Matrix = Mat;
	float Left =
		Matrix.Rc[0][0] * Matrix.Rc[1][1] * Matrix.Rc[2][2] * Matrix.Rc[3][3] +
		Matrix.Rc[0][0] * Matrix.Rc[1][2] * Matrix.Rc[2][3] * Matrix.Rc[3][1] +
		Matrix.Rc[0][0] * Matrix.Rc[1][3] * Matrix.Rc[2][1] * Matrix.Rc[3][2] +

		Matrix.Rc[0][1] * Matrix.Rc[1][0] * Matrix.Rc[2][3] * Matrix.Rc[3][2] +
		Matrix.Rc[0][1] * Matrix.Rc[1][2] * Matrix.Rc[2][0] * Matrix.Rc[3][3] +
		Matrix.Rc[0][1] * Matrix.Rc[1][3] * Matrix.Rc[2][2] * Matrix.Rc[3][0] +

		Matrix.Rc[0][2] * Matrix.Rc[1][0] * Matrix.Rc[2][1] * Matrix.Rc[3][3] +
		Matrix.Rc[0][2] * Matrix.Rc[1][1] * Matrix.Rc[2][3] * Matrix.Rc[3][0] +
		Matrix.Rc[0][2] * Matrix.Rc[1][3] * Matrix.Rc[2][0] * Matrix.Rc[3][1] +

		Matrix.Rc[0][3] * Matrix.Rc[1][0] * Matrix.Rc[2][2] * Matrix.Rc[3][1] +
		Matrix.Rc[0][3] * Matrix.Rc[1][1] * Matrix.Rc[2][0] * Matrix.Rc[3][2] +
		Matrix.Rc[0][3] * Matrix.Rc[1][2] * Matrix.Rc[2][1] * Matrix.Rc[3][0];

	float Right = 
		-Matrix.Rc[0][0] * Matrix.Rc[1][1] * Matrix.Rc[2][3] * Matrix.Rc[3][2] -
		Matrix.Rc[0][0] * Matrix.Rc[1][2] * Matrix.Rc[2][1] * Matrix.Rc[3][3] -
		Matrix.Rc[0][0] * Matrix.Rc[1][3] * Matrix.Rc[2][2] * Matrix.Rc[3][1] -
																			  
		Matrix.Rc[0][1] * Matrix.Rc[1][0] * Matrix.Rc[2][2] * Matrix.Rc[3][3] -
		Matrix.Rc[0][1] * Matrix.Rc[1][2] * Matrix.Rc[2][3] * Matrix.Rc[3][0] -
		Matrix.Rc[0][1] * Matrix.Rc[1][3] * Matrix.Rc[2][0] * Matrix.Rc[3][2] -
																			  
		Matrix.Rc[0][2] * Matrix.Rc[1][0] * Matrix.Rc[2][3] * Matrix.Rc[3][1] -
		Matrix.Rc[0][2] * Matrix.Rc[1][1] * Matrix.Rc[2][0] * Matrix.Rc[3][3] -
		Matrix.Rc[0][2] * Matrix.Rc[1][3] * Matrix.Rc[2][1] * Matrix.Rc[3][0] -
																			  
		Matrix.Rc[0][3] * Matrix.Rc[1][0] * Matrix.Rc[2][1] * Matrix.Rc[3][2] -
		Matrix.Rc[0][3] * Matrix.Rc[1][1] * Matrix.Rc[2][2] * Matrix.Rc[3][0] -
		Matrix.Rc[0][3] * Matrix.Rc[1][2] * Matrix.Rc[2][0] * Matrix.Rc[3][1];

	float Result = Left + Right;
	/*unsigned int Lead = 0;
	unsigned int RowCount = 4;
	unsigned int ColumnCount = 4;
	for (unsigned int Row = 0; Row <= RowCount; Row++)
	{
		if (ColumnCount <= Lead)
		{
			// stop
		}
		unsigned int i = Row;
		while (Matrix.Rc[i][Lead] == 0)
		{
			i = i + 1;
			if (RowCount == i)
			{
				i = Row;
				Lead = Lead + 1;
				if (ColumnCount == Lead)
				{
					// stop
				}
			}
		}
		swap rows i and r;
		if (Matrix.Rc[Row, Lead] != 0)
		{
			Matrix.Rc[Row, 0] = Matrix.Rc[Row, 0] / Matrix.Rc[Row, Lead];
			Matrix.Rc[Row, 1] = Matrix.Rc[Row, 1] / Matrix.Rc[Row, Lead];
			Matrix.Rc[Row, 2] = Matrix.Rc[Row, 2] / Matrix.Rc[Row, Lead];
			Matrix.Rc[Row, 3] = Matrix.Rc[Row, 3] / Matrix.Rc[Row, Lead];
		}
		for (i; i < RowCount; i++)
		{
			if (i != Row)
			{
				Matrix.Rc[i, 0] = Matrix.Rc[i, 0] - Matrix.Rc[i, Lead] * Matrix.Rc[Row, 0];
				Matrix.Rc[i, 1] = Matrix.Rc[i, 1] - Matrix.Rc[i, Lead] * Matrix.Rc[Row, 1];
				Matrix.Rc[i, 2] = Matrix.Rc[i, 2] - Matrix.Rc[i, Lead] * Matrix.Rc[Row, 2];
				Matrix.Rc[i, 3] = Matrix.Rc[i, 3] - Matrix.Rc[i, Lead] * Matrix.Rc[Row, 3];
			}
		}
		Lead = Lead + 1;
	}
	*/
	return Result;
}

m4 MatrixInverse(m4 &Matrix)
{
	m4 Result;

	//Result = (1 / Determinant(Matrix)) * Matrix;

	return Result;
}

/*
GLM_FUNC_QUALIFIER void sse_inverse_ps(__m128 const in[4], __m128 out[4])
{
__m128 Fac0;
{
//	valType SubFactor00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
//	valType SubFactor00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
//	valType SubFactor06 = m[1][2] * m[3][3] - m[3][2] * m[1][3];
//	valType SubFactor13 = m[1][2] * m[2][3] - m[2][2] * m[1][3];

__m128 Swp0a = _mm_shuffle_ps(in[3], in[2], _MM_SHUFFLE(3, 3, 3, 3));
__m128 Swp0b = _mm_shuffle_ps(in[3], in[2], _MM_SHUFFLE(2, 2, 2, 2));

__m128 Swp00 = _mm_shuffle_ps(in[2], in[1], _MM_SHUFFLE(2, 2, 2, 2));
__m128 Swp01 = _mm_shuffle_ps(Swp0a, Swp0a, _MM_SHUFFLE(2, 0, 0, 0));
__m128 Swp02 = _mm_shuffle_ps(Swp0b, Swp0b, _MM_SHUFFLE(2, 0, 0, 0));
__m128 Swp03 = _mm_shuffle_ps(in[2], in[1], _MM_SHUFFLE(3, 3, 3, 3));

__m128 Mul00 = _mm_mul_ps(Swp00, Swp01);
__m128 Mul01 = _mm_mul_ps(Swp02, Swp03);
Fac0 = _mm_sub_ps(Mul00, Mul01);
}

__m128 Fac1;
{
//	valType SubFactor01 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
//	valType SubFactor01 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
//	valType SubFactor07 = m[1][1] * m[3][3] - m[3][1] * m[1][3];
//	valType SubFactor14 = m[1][1] * m[2][3] - m[2][1] * m[1][3];

__m128 Swp0a = _mm_shuffle_ps(in[3], in[2], _MM_SHUFFLE(3, 3, 3, 3));
__m128 Swp0b = _mm_shuffle_ps(in[3], in[2], _MM_SHUFFLE(1, 1, 1, 1));

__m128 Swp00 = _mm_shuffle_ps(in[2], in[1], _MM_SHUFFLE(1, 1, 1, 1));
__m128 Swp01 = _mm_shuffle_ps(Swp0a, Swp0a, _MM_SHUFFLE(2, 0, 0, 0));
__m128 Swp02 = _mm_shuffle_ps(Swp0b, Swp0b, _MM_SHUFFLE(2, 0, 0, 0));
__m128 Swp03 = _mm_shuffle_ps(in[2], in[1], _MM_SHUFFLE(3, 3, 3, 3));

__m128 Mul00 = _mm_mul_ps(Swp00, Swp01);
__m128 Mul01 = _mm_mul_ps(Swp02, Swp03);
Fac1 = _mm_sub_ps(Mul00, Mul01);
}


__m128 Fac2;
{
//	valType SubFactor02 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
//	valType SubFactor02 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
//	valType SubFactor08 = m[1][1] * m[3][2] - m[3][1] * m[1][2];
//	valType SubFactor15 = m[1][1] * m[2][2] - m[2][1] * m[1][2];

__m128 Swp0a = _mm_shuffle_ps(in[3], in[2], _MM_SHUFFLE(2, 2, 2, 2));
__m128 Swp0b = _mm_shuffle_ps(in[3], in[2], _MM_SHUFFLE(1, 1, 1, 1));

__m128 Swp00 = _mm_shuffle_ps(in[2], in[1], _MM_SHUFFLE(1, 1, 1, 1));
__m128 Swp01 = _mm_shuffle_ps(Swp0a, Swp0a, _MM_SHUFFLE(2, 0, 0, 0));
__m128 Swp02 = _mm_shuffle_ps(Swp0b, Swp0b, _MM_SHUFFLE(2, 0, 0, 0));
__m128 Swp03 = _mm_shuffle_ps(in[2], in[1], _MM_SHUFFLE(2, 2, 2, 2));

__m128 Mul00 = _mm_mul_ps(Swp00, Swp01);
__m128 Mul01 = _mm_mul_ps(Swp02, Swp03);
Fac2 = _mm_sub_ps(Mul00, Mul01);
}

__m128 Fac3;
{
//	valType SubFactor03 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
//	valType SubFactor03 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
//	valType SubFactor09 = m[1][0] * m[3][3] - m[3][0] * m[1][3];
//	valType SubFactor16 = m[1][0] * m[2][3] - m[2][0] * m[1][3];

__m128 Swp0a = _mm_shuffle_ps(in[3], in[2], _MM_SHUFFLE(3, 3, 3, 3));
__m128 Swp0b = _mm_shuffle_ps(in[3], in[2], _MM_SHUFFLE(0, 0, 0, 0));

__m128 Swp00 = _mm_shuffle_ps(in[2], in[1], _MM_SHUFFLE(0, 0, 0, 0));
__m128 Swp01 = _mm_shuffle_ps(Swp0a, Swp0a, _MM_SHUFFLE(2, 0, 0, 0));
__m128 Swp02 = _mm_shuffle_ps(Swp0b, Swp0b, _MM_SHUFFLE(2, 0, 0, 0));
__m128 Swp03 = _mm_shuffle_ps(in[2], in[1], _MM_SHUFFLE(3, 3, 3, 3));

__m128 Mul00 = _mm_mul_ps(Swp00, Swp01);
__m128 Mul01 = _mm_mul_ps(Swp02, Swp03);
Fac3 = _mm_sub_ps(Mul00, Mul01);
}

__m128 Fac4;
{
//	valType SubFactor04 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
//	valType SubFactor04 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
//	valType SubFactor10 = m[1][0] * m[3][2] - m[3][0] * m[1][2];
//	valType SubFactor17 = m[1][0] * m[2][2] - m[2][0] * m[1][2];

__m128 Swp0a = _mm_shuffle_ps(in[3], in[2], _MM_SHUFFLE(2, 2, 2, 2));
__m128 Swp0b = _mm_shuffle_ps(in[3], in[2], _MM_SHUFFLE(0, 0, 0, 0));

__m128 Swp00 = _mm_shuffle_ps(in[2], in[1], _MM_SHUFFLE(0, 0, 0, 0));
__m128 Swp01 = _mm_shuffle_ps(Swp0a, Swp0a, _MM_SHUFFLE(2, 0, 0, 0));
__m128 Swp02 = _mm_shuffle_ps(Swp0b, Swp0b, _MM_SHUFFLE(2, 0, 0, 0));
__m128 Swp03 = _mm_shuffle_ps(in[2], in[1], _MM_SHUFFLE(2, 2, 2, 2));

__m128 Mul00 = _mm_mul_ps(Swp00, Swp01);
__m128 Mul01 = _mm_mul_ps(Swp02, Swp03);
Fac4 = _mm_sub_ps(Mul00, Mul01);
}

__m128 Fac5;
{
//	valType SubFactor05 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
//	valType SubFactor05 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
//	valType SubFactor12 = m[1][0] * m[3][1] - m[3][0] * m[1][1];
//	valType SubFactor18 = m[1][0] * m[2][1] - m[2][0] * m[1][1];

__m128 Swp0a = _mm_shuffle_ps(in[3], in[2], _MM_SHUFFLE(1, 1, 1, 1));
__m128 Swp0b = _mm_shuffle_ps(in[3], in[2], _MM_SHUFFLE(0, 0, 0, 0));

__m128 Swp00 = _mm_shuffle_ps(in[2], in[1], _MM_SHUFFLE(0, 0, 0, 0));
__m128 Swp01 = _mm_shuffle_ps(Swp0a, Swp0a, _MM_SHUFFLE(2, 0, 0, 0));
__m128 Swp02 = _mm_shuffle_ps(Swp0b, Swp0b, _MM_SHUFFLE(2, 0, 0, 0));
__m128 Swp03 = _mm_shuffle_ps(in[2], in[1], _MM_SHUFFLE(1, 1, 1, 1));

__m128 Mul00 = _mm_mul_ps(Swp00, Swp01);
__m128 Mul01 = _mm_mul_ps(Swp02, Swp03);
Fac5 = _mm_sub_ps(Mul00, Mul01);
}

__m128 SignA = _mm_set_ps( 1.0f,-1.0f, 1.0f,-1.0f);
__m128 SignB = _mm_set_ps(-1.0f, 1.0f,-1.0f, 1.0f);

// m[1][0]
// m[0][0]
// m[0][0]
// m[0][0]
__m128 Temp0 = _mm_shuffle_ps(in[1], in[0], _MM_SHUFFLE(0, 0, 0, 0));
__m128 Vec0 = _mm_shuffle_ps(Temp0, Temp0, _MM_SHUFFLE(2, 2, 2, 0));

// m[1][1]
// m[0][1]
// m[0][1]
// m[0][1]
__m128 Temp1 = _mm_shuffle_ps(in[1], in[0], _MM_SHUFFLE(1, 1, 1, 1));
__m128 Vec1 = _mm_shuffle_ps(Temp1, Temp1, _MM_SHUFFLE(2, 2, 2, 0));

// m[1][2]
// m[0][2]
// m[0][2]
// m[0][2]
__m128 Temp2 = _mm_shuffle_ps(in[1], in[0], _MM_SHUFFLE(2, 2, 2, 2));
__m128 Vec2 = _mm_shuffle_ps(Temp2, Temp2, _MM_SHUFFLE(2, 2, 2, 0));

// m[1][3]
// m[0][3]
// m[0][3]
// m[0][3]
__m128 Temp3 = _mm_shuffle_ps(in[1], in[0], _MM_SHUFFLE(3, 3, 3, 3));
__m128 Vec3 = _mm_shuffle_ps(Temp3, Temp3, _MM_SHUFFLE(2, 2, 2, 0));

// col0
// + (Vec1[0] * Fac0[0] - Vec2[0] * Fac1[0] + Vec3[0] * Fac2[0]),
// - (Vec1[1] * Fac0[1] - Vec2[1] * Fac1[1] + Vec3[1] * Fac2[1]),
// + (Vec1[2] * Fac0[2] - Vec2[2] * Fac1[2] + Vec3[2] * Fac2[2]),
// - (Vec1[3] * Fac0[3] - Vec2[3] * Fac1[3] + Vec3[3] * Fac2[3]),
__m128 Mul00 = _mm_mul_ps(Vec1, Fac0);
__m128 Mul01 = _mm_mul_ps(Vec2, Fac1);
__m128 Mul02 = _mm_mul_ps(Vec3, Fac2);
__m128 Sub00 = _mm_sub_ps(Mul00, Mul01);
__m128 Add00 = _mm_add_ps(Sub00, Mul02);
__m128 Inv0 = _mm_mul_ps(SignB, Add00);

// col1
// - (Vec0[0] * Fac0[0] - Vec2[0] * Fac3[0] + Vec3[0] * Fac4[0]),
// + (Vec0[0] * Fac0[1] - Vec2[1] * Fac3[1] + Vec3[1] * Fac4[1]),
// - (Vec0[0] * Fac0[2] - Vec2[2] * Fac3[2] + Vec3[2] * Fac4[2]),
// + (Vec0[0] * Fac0[3] - Vec2[3] * Fac3[3] + Vec3[3] * Fac4[3]),
__m128 Mul03 = _mm_mul_ps(Vec0, Fac0);
__m128 Mul04 = _mm_mul_ps(Vec2, Fac3);
__m128 Mul05 = _mm_mul_ps(Vec3, Fac4);
__m128 Sub01 = _mm_sub_ps(Mul03, Mul04);
__m128 Add01 = _mm_add_ps(Sub01, Mul05);
__m128 Inv1 = _mm_mul_ps(SignA, Add01);

// col2
// + (Vec0[0] * Fac1[0] - Vec1[0] * Fac3[0] + Vec3[0] * Fac5[0]),
// - (Vec0[0] * Fac1[1] - Vec1[1] * Fac3[1] + Vec3[1] * Fac5[1]),
// + (Vec0[0] * Fac1[2] - Vec1[2] * Fac3[2] + Vec3[2] * Fac5[2]),
// - (Vec0[0] * Fac1[3] - Vec1[3] * Fac3[3] + Vec3[3] * Fac5[3]),
__m128 Mul06 = _mm_mul_ps(Vec0, Fac1);
__m128 Mul07 = _mm_mul_ps(Vec1, Fac3);
__m128 Mul08 = _mm_mul_ps(Vec3, Fac5);
__m128 Sub02 = _mm_sub_ps(Mul06, Mul07);
__m128 Add02 = _mm_add_ps(Sub02, Mul08);
__m128 Inv2 = _mm_mul_ps(SignB, Add02);

// col3
// - (Vec1[0] * Fac2[0] - Vec1[0] * Fac4[0] + Vec2[0] * Fac5[0]),
// + (Vec1[0] * Fac2[1] - Vec1[1] * Fac4[1] + Vec2[1] * Fac5[1]),
// - (Vec1[0] * Fac2[2] - Vec1[2] * Fac4[2] + Vec2[2] * Fac5[2]),
// + (Vec1[0] * Fac2[3] - Vec1[3] * Fac4[3] + Vec2[3] * Fac5[3]));
__m128 Mul09 = _mm_mul_ps(Vec0, Fac2);
__m128 Mul10 = _mm_mul_ps(Vec1, Fac4);
__m128 Mul11 = _mm_mul_ps(Vec2, Fac5);
__m128 Sub03 = _mm_sub_ps(Mul09, Mul10);
__m128 Add03 = _mm_add_ps(Sub03, Mul11);
__m128 Inv3 = _mm_mul_ps(SignA, Add03);

__m128 Row0 = _mm_shuffle_ps(Inv0, Inv1, _MM_SHUFFLE(0, 0, 0, 0));
__m128 Row1 = _mm_shuffle_ps(Inv2, Inv3, _MM_SHUFFLE(0, 0, 0, 0));
__m128 Row2 = _mm_shuffle_ps(Row0, Row1, _MM_SHUFFLE(2, 0, 2, 0));

//	valType Determinant = m[0][0] * Inverse[0][0]
//						+ m[0][1] * Inverse[1][0]
//						+ m[0][2] * Inverse[2][0]
//						+ m[0][3] * Inverse[3][0];
__m128 Det0 = sse_dot_ps(in[0], Row2);
__m128 Rcp0 = _mm_div_ps(one, Det0);
//__m128 Rcp0 = _mm_rcp_ps(Det0);

//	Inverse /= Determinant;
out[0] = _mm_mul_ps(Inv0, Rcp0);
out[1] = _mm_mul_ps(Inv1, Rcp0);
out[2] = _mm_mul_ps(Inv2, Rcp0);
out[3] = _mm_mul_ps(Inv3, Rcp0);
}
*/