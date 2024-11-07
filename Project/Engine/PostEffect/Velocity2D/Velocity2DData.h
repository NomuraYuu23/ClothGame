#pragma once
#include "../../Math/Vector/Vector2.h"
#include "../../Math/Vector/Vector3.h"
#include "../../Math/Matrix/Matrix4x4.h"

struct Velocity2DData
{
	// 速度
	Vector2 velocity;

	/// <summary>
	/// 速度設定
	/// </summary>
	/// <param name="velocity3D">3Dでの速度</param>
	/// <param name="viewMatrix">ビュー行列</param>
	void SetVelocity(
		const Vector3& velocity3D,
		const Matrix4x4& viewMatrix);

};
