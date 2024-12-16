#pragma once

/// <summary>
/// 数学系関数
/// </summary>
class Math
{

public: // メンバ関数

	/// <summary>
	/// 補間
	/// </summary>
	/// <param name="start">開始</param>
	/// <param name="end">終了</param>
	/// <param name="t">係数</param>
	/// <returns></returns>
	static float Lerp(float start, float end, float t);

	/// <summary>
	/// 最短角度補間
	/// </summary>
	/// <param name="start">開始</param>
	/// <param name="end">終了</param>
	/// <param name="t">係数</param>
	/// <returns></returns>
	static float LerpShortAngle(float start, float end, float t);

public: // メンバ変数

	// π
	static const float PI;

};

