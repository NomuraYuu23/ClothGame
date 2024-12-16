#pragma once
#include "../Math/Vector/Vector3.h"

/// <summary>
/// 線分
/// </summary>
class Segment
{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="origin">原点</param>
	/// <param name="diff">終点までの差分ベクトル</param>
	void Initialize(const Vector3& origin, const Vector3& diff);

	/// <summary>
	/// 次の地点へ
	/// </summary>
	/// <param name="point">点</param>
	/// <param name="segment">線分</param>
	/// <returns></returns>
	Vector3 ClosestPoint(const Vector3& point, const Segment& segment);

public: // メンバ変数
	
	// 原点
	Vector3 origin_;
	// 終点までの差分ベクトル
	Vector3 diff_;

};

