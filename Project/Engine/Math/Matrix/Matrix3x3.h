#pragma once
#include "../Vector/Vector2.h"

class Matrix3x3
{

public: // 変数

	float m[3][3];

public: // 関数

	//平行移動行列生成関数
	static Matrix3x3 MakeTranslateMatrix(Vector2 trnslate);
	//回転行列生成関数
	static Matrix3x3 MakeRotateMatrix(float theta);
	//行列3x3の積
	static Matrix3x3 Multiply(Matrix3x3 matrix1, Matrix3x3 matrix2);
	//移動関数
	static Vector2 Transform(Vector2 vector, Matrix3x3 matrix);
	//逆行列
	static Matrix3x3 Inverse(Matrix3x3 matrix);

};

