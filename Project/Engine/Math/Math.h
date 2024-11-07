#pragma once
class Math
{

public: // メンバ関数

	// 補間
	static float Lerp(float start, float end, float t);

	// 最短角度補間
	static float LerpShortAngle(float start, float end, float t);

public: // メンバ変数

	// π
	static const float PI;

};

