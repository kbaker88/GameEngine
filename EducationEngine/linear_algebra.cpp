#include "linear_algebra.h"

m4 
operator*(m4 &A, m4 &B)
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

v3
Math_CrossProduct(v3 &A, v3 &B)
{
	v3 Result = {};
	Result.x = (A.y * B.z) - (A.z * B.y);
	Result.y = (A.z * B.x) - (A.x * B.z);
	Result.z = (A.x * B.y) - (A.y * B.x);

	return Result;
}

v3
Math_ReflectVector(v3 &IncomingVect, v3 &Normal)
{
	v3 Result;
	Result = IncomingVect - 2.0f * Math_InnerProduct(&IncomingVect, &Normal) * Normal;

	return Result;
}


m4 
Math_IdentityMatrix()
{
	m4 Result;
	
	Result.Rc[0][0] = 1.0f;
	Result.Rc[1][1] = 1.0f;
	Result.Rc[2][2] = 1.0f;
	Result.Rc[3][3] = 1.0f;

	return Result;
}

m4 
Math_LookAtMatrix(v3 &Eye, v3 &Target, v3 &Up)
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

m4
Math_PerspectiveMatrix(float FieldOfView, float AspectRatio, float NearPlane, float FarPlane)
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

m4
Math_OrthographicMarix(float Left, float Right, float Bottom, float Top,
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

m4
Math_TranslateMatrix(m4 &Matrix, v3 &Vector)
{
	m4 Result = Matrix;

	Result.Rc[3][0] += Vector.Arr[0];
	Result.Rc[3][1] += Vector.Arr[1];
	Result.Rc[3][2] += Vector.Arr[2];

	return Result;
}

m4 
Math_ScaleMatrix(m4 &Matrix, v3 &Vector)
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
v4 
forwardSub(m4 ALower, v4 B) {
	v4 X;
	X.x = B.x / ALower.Rc[0][0];
	X.y = (B.y - (X.x * ALower.Rc[0][1])) / ALower.Rc[1][1];
	X.z = (B.z - (X.x * ALower.Rc[0][2] + X.y * ALower.Rc[1][2])) / ALower.Rc[2][2];
	X.w = (B.w - (X.x * ALower.Rc[0][3] + X.y * ALower.Rc[1][3] + X.z * ALower.Rc[2][3])) / ALower.Rc[3][3];
	return X;
}

// solves the system Ux = B when U is an upper triangular matrix.
v4
backwardSub(m4 AUpper, v4 B) {
	v4 X;
	X.w = B.w / AUpper.Rc[3][3];
	X.z = (B.z - (X.w * AUpper.Rc[3][2])) / AUpper.Rc[2][2];
	X.y = (B.y - (X.z * AUpper.Rc[2][1] + X.w * AUpper.Rc[3][1])) / AUpper.Rc[1][1];
	X.x = (B.x - (X.y * AUpper.Rc[1][0] + X.z * AUpper.Rc[2][0] + X.w * AUpper.Rc[3][0])) / AUpper.Rc[0][0];
	return X;
}

// swaps rows m and n of an m4 matrix then returns the result.
m4 
rowInterchange(m4 Matrix, int m, int n)
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
m4 
columnInterchange(m4 Matrix, int m, int n)
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
v4 
applyPermutation(int P[4], v4 I) {
	v4 result;
	for (int i = 0; i < 4; i++) {
		result.Arr[i] = I.Arr[P[i] - 1];
	}
	return result;
}

m4 
Math_InvertMatrix(m4 *Matrix)
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