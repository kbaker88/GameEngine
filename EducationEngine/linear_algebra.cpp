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

v3 Math_CrossProduct(v3 &A, v3 &B)
{
	v3 Result = {};
	Result.x = (A.y * B.z) - (A.z * B.y);
	Result.y = (A.z * B.x) - (A.x * B.z);
	Result.z = (A.x * B.y) - (A.y * B.x);

	return Result;
}

v3 Math_ReflectVector(v3 &IncomingVect, v3 &Normal)
{
	v3 Result;
	Result = IncomingVect - 2.0f * Math_InnerProduct(&IncomingVect, &Normal) * Normal;

	return Result;
}


m4 Math_IdentityMatrix()
{
	m4 Result;
	
	Result.Rc[0][0] = 1.0f;
	Result.Rc[1][1] = 1.0f;
	Result.Rc[2][2] = 1.0f;
	Result.Rc[3][3] = 1.0f;

	return Result;
}

m4 Math_LookAtMatrix(v3 &Eye, v3 &Target, v3 &Up)
{
	v3 ZAxis = Math_Normalize(Target - Eye);
	v3 XAxis = Math_Normalize(Math_CrossProduct(ZAxis, Up));
	v3 YAxis = Math_CrossProduct(XAxis, ZAxis);

	m4 Result;
	Result.Rc[0][0] = XAxis.x;
	Result.Rc[1][0] = XAxis.y;
	Result.Rc[2][0] = XAxis.z;
	Result.Rc[0][1] = YAxis.x;
	Result.Rc[1][1] = YAxis.y;
	Result.Rc[2][1] = YAxis.z;
	Result.Rc[0][2] = -ZAxis.x;
	Result.Rc[1][2] = -ZAxis.y;
	Result.Rc[2][2] = -ZAxis.z;
	Result.Rc[3][0] = -Math_InnerProduct(&XAxis, &Eye);
	Result.Rc[3][1] = -Math_InnerProduct(&YAxis, &Eye);
	Result.Rc[3][2] = Math_InnerProduct(&ZAxis, &Eye);
	Result.Rc[3][3] = 1.0f;
	return Result;
}

m4 Math_PerspectiveMatrix(float FieldOfView, float AspectRatio, float NearPlane, float FarPlane)
{
	m4 Matrix;
	float HalfVal = (float)Math_Tangent(FieldOfView * 0.5f);

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

m4 Math_OrthographicMarix(float Left, float Right, float Bottom, float Top,
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

m4 Math_TranslateMatrix(m4 &Matrix, v3 &Vector)
{
	m4 Result = Matrix;

	Result.Rc[3][0] += Vector.Arr[0];
	Result.Rc[3][1] += Vector.Arr[1];
	Result.Rc[3][2] += Vector.Arr[2];

	return Result;
}

m4 Math_ScaleMatrix(m4 &Matrix, v3 &Vector)
{
	m4 Result;

	Result.Rc[0][0] = Matrix.Rc[0][0] * Vector.Arr[0];
	Result.Rc[1][1] = Matrix.Rc[1][1] * Vector.Arr[1];
	Result.Rc[2][2] = Matrix.Rc[2][2] * Vector.Arr[2];
	Result.Rc[3][3] = Matrix.Rc[3][3];
	return Result;
}

// Temp for invert matrix
/////////////////////////
/////////////////////////
v4 forwardSub(m4 ALower, v4 B) {
	v4 X;
	X.x = B.x / ALower.Rc[0][0];
	X.y = (B.y - (X.x * ALower.Rc[0][1])) / ALower.Rc[1][1];
	X.z = (B.z - (X.x * ALower.Rc[0][2] + X.y * ALower.Rc[1][2])) / ALower.Rc[2][2];
	X.w = (B.w - (X.x * ALower.Rc[0][3] + X.y * ALower.Rc[1][3] + X.z * ALower.Rc[2][3])) / ALower.Rc[3][3];
	return X;
}

// solves the system Ux = B when U is an upper triangular matrix.
v4 backwardSub(m4 AUpper, v4 B) {
	v4 X;
	X.w = B.w / AUpper.Rc[3][3];
	X.z = (B.z - (X.w * AUpper.Rc[3][2])) / AUpper.Rc[2][2];
	X.y = (B.y - (X.z * AUpper.Rc[2][1] + X.w * AUpper.Rc[3][1])) / AUpper.Rc[1][1];
	X.x = (B.x - (X.y * AUpper.Rc[1][0] + X.z * AUpper.Rc[2][0] + X.w * AUpper.Rc[3][0])) / AUpper.Rc[0][0];
	return X;
}

// swaps rows m and n of an m4 matrix then returns the result.
m4 rowInterchange(m4 Matrix, int m, int n)
{
	m4 result = Matrix;
	if (0 > m) {
		// TODO: report operation did not succeed
		return result;
	}
	else if (0 > n) {
		// TODO: report operation did not succeed
		return result;
	}
	else {
		for (int j = 0; j < 4; j++)
		{
			for (int i = 0; i < 4; i++)
			{
				if (j == m) {
					result.Rc[i][j] = Matrix.Rc[i][n];
				}
				else if (j == n) {
					result.Rc[i][j] = Matrix.Rc[i][m];
				}
			}
		}
	}
	return result;
}

// swaps columns m and n of an m4 matrix then returns the result.
m4 columnInterchange(m4 Matrix, int m, int n)
{
	m4 result = Matrix;
	if (0 > m) {
		// TODO: report operation did not succeed
		return result;
	}
	else if (0 > n) {
		// TODO: report operation did not succeed
		return result;
	}
	else {
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if (i == m) {
					result.Rc[i][j] = Matrix.Rc[n][j];
				}
				else if (i == n) {
					result.Rc[i][j] = Matrix.Rc[m][j];
				}
			}
		}
	}
	return result;
}

// takes an array P that represents some permutation of {1, 2, 3, 4} and a vector I.
// Reorders I according to the permutation stored in P.
v4 applyPermutation(int P[4], v4 I) {
	v4 result;
	for (int i = 0; i < 4; i++) {
		result.Arr[i] = I.Arr[P[i] - 1];
	}
	return result;
}

m4 Math_InvertMatrix(m4 *Matrix)
{
	m4 matrix = *Matrix;

	m4 U = *Matrix;

	// the L matrix will be zeroed out initially and then populated as a strictly lower
	// triangular matrix with the following form:
	// | 0 0 0 0 |
	// | a 0 0 0 |
	// | b c 0 0 |
	// | d e f 0 |
	m4 L;
	for (int x = 0; x < 4; x++) {
		L.Rc[x][x] = 0;
	}
	// after all row swap operations have been performed during the population of the U
	// matrix, the 1's will be introduced back into L's main diagonal.

	// P is a vector that keeps track of the permutation of rows among our matrices L and U.
	// example: if we swapped rows 1 and 2 of L and U, P would look like { 2, 1, 3, 4 }
	int P[4] = { 1, 2, 3, 4 };

	// initiate the PA = LU factorization algorithm.
	for (int x = 0; x < 4; x++) {
		// if row swapping becomes necessary, perform a row swap and "factor" out
		// a permutation matrix.
		if (matrix.Rc[x][x] == 0) {
			bool rowSwapped = false;
			for (int y = x + 1; y < 4; y++) {
				if (!rowSwapped) {
					if (matrix.Rc[x][y] != 0) {
						L = rowInterchange(L, x, y);
						U = rowInterchange(U, x, y);
						int temp = P[x];
						P[x] = P[y];
						P[y] = temp;
						rowSwapped = true;
						matrix = U;
					}
				}
			}
		}
		// The loop below uses matrix row operations to turn U into a
		// true upper triangular matrix, generating L along the way.
		for (int i = x + 1; i < 4; i++) {
			// scaleRow is the scaling factor that will zero out the element below x,y
			float scaleRow = (matrix.Rc[x][i] / matrix.Rc[x][x]);
			// populate L with the scaling factors used row by row to generate U.
			L.Rc[x][i] = scaleRow;
			for (int j = 0; j < 4; j++) {
				U.Rc[j][i] = matrix.Rc[j][i] - (matrix.Rc[j][x] * scaleRow);
				if (Math_AbsVal(U.Rc[j][i]) < 0.000001) {
					U.Rc[j][i] = 0;
				}
			}
			matrix = U;
		}
	}

	// insert 1's along the main diagonal of L, making it into a normal lower triangular matrix.
	for (int x = 0; x < 4; x++) {
		L.Rc[x][x] = 1;
	}
	m4 Ainv;

	// solving the system for the first column of the identity, appropriately permuted if necessary.
	v4 D1 = forwardSub(L, applyPermutation(P, v4(1, 0, 0, 0)));
	v4 B1 = backwardSub(U, D1);
	Ainv.Rc[0][0] = B1.x;
	Ainv.Rc[0][1] = B1.y;
	Ainv.Rc[0][2] = B1.z;
	Ainv.Rc[0][3] = B1.w;

	// solving the system for the second column of the identity, appropriately permuted if necessary.
	v4 D2 = forwardSub(L, applyPermutation(P, v4(0, 1, 0, 0)));
	v4 B2 = backwardSub(U, D2);
	Ainv.Rc[1][0] = B2.x;
	Ainv.Rc[1][1] = B2.y;
	Ainv.Rc[1][2] = B2.z;
	Ainv.Rc[1][3] = B2.w;

	// solving the system for the third column of the identity, appropriately permuted if necessary.
	v4 D3 = forwardSub(L, applyPermutation(P, v4(0, 0, 1, 0)));
	v4 B3 = backwardSub(U, D3);
	Ainv.Rc[2][0] = B3.x;
	Ainv.Rc[2][1] = B3.y;
	Ainv.Rc[2][2] = B3.z;
	Ainv.Rc[2][3] = B3.w;

	// solving the system for the fourth column of the identity, appropriately permuted if necessary.
	v4 D4 = forwardSub(L, applyPermutation(P, v4(0, 0, 0, 1)));
	v4 B4 = backwardSub(U, D4);
	Ainv.Rc[3][0] = B4.x;
	Ainv.Rc[3][1] = B4.y;
	Ainv.Rc[3][2] = B4.z;
	Ainv.Rc[3][3] = B4.w;

	return Ainv;
}

//float Determinant(m4 &Mat)
//{
//	m4 Matrix = Mat;
//	float Left =
//		Matrix.Rc[0][0] * Matrix.Rc[1][1] * Matrix.Rc[2][2] * Matrix.Rc[3][3] +
//		Matrix.Rc[0][0] * Matrix.Rc[1][2] * Matrix.Rc[2][3] * Matrix.Rc[3][1] +
//		Matrix.Rc[0][0] * Matrix.Rc[1][3] * Matrix.Rc[2][1] * Matrix.Rc[3][2] +
//
//		Matrix.Rc[0][1] * Matrix.Rc[1][0] * Matrix.Rc[2][3] * Matrix.Rc[3][2] +
//		Matrix.Rc[0][1] * Matrix.Rc[1][2] * Matrix.Rc[2][0] * Matrix.Rc[3][3] +
//		Matrix.Rc[0][1] * Matrix.Rc[1][3] * Matrix.Rc[2][2] * Matrix.Rc[3][0] +
//
//		Matrix.Rc[0][2] * Matrix.Rc[1][0] * Matrix.Rc[2][1] * Matrix.Rc[3][3] +
//		Matrix.Rc[0][2] * Matrix.Rc[1][1] * Matrix.Rc[2][3] * Matrix.Rc[3][0] +
//		Matrix.Rc[0][2] * Matrix.Rc[1][3] * Matrix.Rc[2][0] * Matrix.Rc[3][1] +
//
//		Matrix.Rc[0][3] * Matrix.Rc[1][0] * Matrix.Rc[2][2] * Matrix.Rc[3][1] +
//		Matrix.Rc[0][3] * Matrix.Rc[1][1] * Matrix.Rc[2][0] * Matrix.Rc[3][2] +
//		Matrix.Rc[0][3] * Matrix.Rc[1][2] * Matrix.Rc[2][1] * Matrix.Rc[3][0];
//
//	float Right = 
//		-Matrix.Rc[0][0] * Matrix.Rc[1][1] * Matrix.Rc[2][3] * Matrix.Rc[3][2] -
//		Matrix.Rc[0][0] * Matrix.Rc[1][2] * Matrix.Rc[2][1] * Matrix.Rc[3][3] -
//		Matrix.Rc[0][0] * Matrix.Rc[1][3] * Matrix.Rc[2][2] * Matrix.Rc[3][1] -
//																			  
//		Matrix.Rc[0][1] * Matrix.Rc[1][0] * Matrix.Rc[2][2] * Matrix.Rc[3][3] -
//		Matrix.Rc[0][1] * Matrix.Rc[1][2] * Matrix.Rc[2][3] * Matrix.Rc[3][0] -
//		Matrix.Rc[0][1] * Matrix.Rc[1][3] * Matrix.Rc[2][0] * Matrix.Rc[3][2] -
//																			  
//		Matrix.Rc[0][2] * Matrix.Rc[1][0] * Matrix.Rc[2][3] * Matrix.Rc[3][1] -
//		Matrix.Rc[0][2] * Matrix.Rc[1][1] * Matrix.Rc[2][0] * Matrix.Rc[3][3] -
//		Matrix.Rc[0][2] * Matrix.Rc[1][3] * Matrix.Rc[2][1] * Matrix.Rc[3][0] -
//																			  
//		Matrix.Rc[0][3] * Matrix.Rc[1][0] * Matrix.Rc[2][1] * Matrix.Rc[3][2] -
//		Matrix.Rc[0][3] * Matrix.Rc[1][1] * Matrix.Rc[2][2] * Matrix.Rc[3][0] -
//		Matrix.Rc[0][3] * Matrix.Rc[1][2] * Matrix.Rc[2][0] * Matrix.Rc[3][1];
//
//	float Result = Left + Right;
//	/*unsigned int Lead = 0;
//	unsigned int RowCount = 4;
//	unsigned int ColumnCount = 4;
//	for (unsigned int Row = 0; Row <= RowCount; Row++)
//	{
//		if (ColumnCount <= Lead)
//		{
//			// stop
//		}
//		unsigned int i = Row;
//		while (Matrix.Rc[i][Lead] == 0)
//		{
//			i = i + 1;
//			if (RowCount == i)
//			{
//				i = Row;
//				Lead = Lead + 1;
//				if (ColumnCount == Lead)
//				{
//					// stop
//				}
//			}
//		}
//		swap rows i and r;
//		if (Matrix.Rc[Row, Lead] != 0)
//		{
//			Matrix.Rc[Row, 0] = Matrix.Rc[Row, 0] / Matrix.Rc[Row, Lead];
//			Matrix.Rc[Row, 1] = Matrix.Rc[Row, 1] / Matrix.Rc[Row, Lead];
//			Matrix.Rc[Row, 2] = Matrix.Rc[Row, 2] / Matrix.Rc[Row, Lead];
//			Matrix.Rc[Row, 3] = Matrix.Rc[Row, 3] / Matrix.Rc[Row, Lead];
//		}
//		for (i; i < RowCount; i++)
//		{
//			if (i != Row)
//			{
//				Matrix.Rc[i, 0] = Matrix.Rc[i, 0] - Matrix.Rc[i, Lead] * Matrix.Rc[Row, 0];
//				Matrix.Rc[i, 1] = Matrix.Rc[i, 1] - Matrix.Rc[i, Lead] * Matrix.Rc[Row, 1];
//				Matrix.Rc[i, 2] = Matrix.Rc[i, 2] - Matrix.Rc[i, Lead] * Matrix.Rc[Row, 2];
//				Matrix.Rc[i, 3] = Matrix.Rc[i, 3] - Matrix.Rc[i, Lead] * Matrix.Rc[Row, 3];
//			}
//		}
//		Lead = Lead + 1;
//	}
//	*/
//	return Result;
//}
//
//m4 MatrixInverse(m4 &Matrix)
//{
//	m4 Result;
//
//	//Result = (1 / Determinant(Matrix)) * Matrix;
//
//	return Result;
//}

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