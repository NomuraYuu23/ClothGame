#pragma once
#include "../Vector/Vector2.h"

/// <summary>
/// 行列3x3
/// </summary>
class Matrix3x3
{

public: // 変数

	float m[3][3];

public: // 関数

	/// <summary>
	/// 平行移動行列生成関数
	/// </summary>
	/// <param name="trnslate">位置</param>
	/// <returns></returns>
	static Matrix3x3 MakeTranslateMatrix(Vector2 trnslate);
	/// <summary>
	/// 回転行列生成関数
	/// </summary>
	/// <param name="theta">回転角</param>
	/// <returns></returns>
	static Matrix3x3 MakeRotateMatrix(float theta);
	/// <summary>
	/// 行列3x3の積
	/// </summary>
	/// <param name="matrix1">一個目の値</param>
	/// <param name="matrix2">二個目の値</param>
	/// <returns></returns>
	static Matrix3x3 Multiply(Matrix3x3 matrix1, Matrix3x3 matrix2);
	/// <summary>
	/// 移動関数
	/// </summary>
	/// <param name="vector">ベクトル</param>
	/// <param name="matrix">行列</param>
	/// <returns></returns>
	static Vector2 Transform(Vector2 vector, Matrix3x3 matrix);
	/// <summary>
	/// 逆行列
	/// </summary>
	/// <param name="matrix">行列</param>
	/// <returns></returns>
	static Matrix3x3 Inverse(Matrix3x3 matrix);

};

