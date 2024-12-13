#include "Matrix3x3.h"

//平行移動行列生成関数
Matrix3x3 Matrix3x3::MakeTranslateMatrix(Vector2 translate) {

	Matrix3x3 result = { 0 };

	for (int y = 0; y < 3; y++) {
		for (int x = 0; x < 3; x++) {

			if (y == x) {
				result.m[y][x] = 1;
			}

			else if (y == 2) {
				if (x == 0) {
					result.m[y][x] = translate.x;
				}
				else {
					result.m[y][x] = translate.y;
				}
			}

			else {
				result.m[y][x] = 0;
			}

		}
	}

	return result;

}

//回転行列生成関数
Matrix3x3 Matrix3x3::MakeRotateMatrix(float theta) {

	Matrix3x3 result = { 0 };

	result.m[0][0] = cosf(theta);
	result.m[0][1] = sinf(theta);
	result.m[0][2] = 0;
	result.m[1][0] = -sinf(theta);
	result.m[1][1] = cosf(theta);
	result.m[1][2] = 0;
	result.m[2][0] = 0;
	result.m[2][1] = 0;
	result.m[2][2] = 1;

	return result;

}

//行列3x3の積
Matrix3x3 Matrix3x3::Multiply(Matrix3x3 matrix1, Matrix3x3 matrix2) {

	Matrix3x3 result = { 0 };

	for (int y = 0; y < 3; y++) {
		for (int x = 0; x < 3; x++) {

			result.m[y][x] = matrix1.m[y][0] * matrix2.m[0][x] + matrix1.m[y][1] * matrix2.m[1][x] + matrix1.m[y][2] * matrix2.m[2][x];

		}
	}

	return result;

}

//移動関数
Vector2 Matrix3x3::Transform(Vector2 vector, Matrix3x3 matrix) {

	Vector2 result = { 0 };

	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + 1.0f * matrix.m[2][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + 1.0f * matrix.m[2][1];
	float w = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + 1.0f * matrix.m[2][2];
	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	return result;

}

//逆行列
Matrix3x3 Matrix3x3::Inverse(Matrix3x3 matrix) {

	Matrix3x3 inverseMatrix;

	float checkExistence = matrix.m[0][0] * matrix.m[1][1] * matrix.m[2][2] + matrix.m[0][1] * matrix.m[1][2] * matrix.m[2][0] + matrix.m[0][2] * matrix.m[1][0] * matrix.m[2][1]
		- matrix.m[0][2] * matrix.m[1][1] * matrix.m[2][0] - matrix.m[0][1] * matrix.m[1][0] * matrix.m[2][2] - matrix.m[0][0] * matrix.m[1][2] * matrix.m[2][1];
	if (checkExistence != 0) {
		inverseMatrix.m[0][0] = (matrix.m[1][1] * matrix.m[2][2] - matrix.m[1][2] * matrix.m[2][1]) / checkExistence;
		inverseMatrix.m[0][1] = (-(matrix.m[0][1] * matrix.m[2][2] - matrix.m[0][2] * matrix.m[2][1])) / checkExistence;
		inverseMatrix.m[0][2] = (matrix.m[0][1] * matrix.m[1][2] - matrix.m[0][2] * matrix.m[1][1]) / checkExistence;
		inverseMatrix.m[1][0] = (-(matrix.m[1][0] * matrix.m[2][2] - matrix.m[1][2] * matrix.m[2][0])) / checkExistence;
		inverseMatrix.m[1][1] = (matrix.m[0][0] * matrix.m[2][2] - matrix.m[0][2] * matrix.m[2][0]) / checkExistence;
		inverseMatrix.m[1][2] = (-(matrix.m[0][0] * matrix.m[1][2] - matrix.m[0][2] * matrix.m[1][0])) / checkExistence;
		inverseMatrix.m[2][0] = (matrix.m[1][0] * matrix.m[2][1] - matrix.m[1][1] * matrix.m[2][0]) / checkExistence;
		inverseMatrix.m[2][1] = (-(matrix.m[0][0] * matrix.m[2][1] - matrix.m[0][1] * matrix.m[2][0])) / checkExistence;
		inverseMatrix.m[2][2] = (matrix.m[0][0] * matrix.m[1][1] - matrix.m[0][1] * matrix.m[1][0]) / checkExistence;
	}
	else {
		for (int y = 0; y < 3; y++) {
			for (int x = 0; x < 3; x++) {
				inverseMatrix.m[y][x] = 0.0f;
			}
		}
	}

	return inverseMatrix;

}
