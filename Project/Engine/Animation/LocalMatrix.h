#pragma once
#include "../Math/Matrix/Matrix4x4.h"

/// <summary>
/// ローカル行列
/// </summary>
struct LocalMatrix
{

	Matrix4x4 matrix; // 行列
	Matrix4x4 matrixInverseTranspose; // 逆転置行列

};