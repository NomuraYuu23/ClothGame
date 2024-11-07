#pragma once
#include "../../Math/Matrix/Matrix4x4.h"

class InertiaTensor
{

public: // 関数

	/// <summary>
	/// 直方体の慣性テンソル
	/// </summary>
	/// <param name="mass">質量</param>
	/// <param name="sideLength">辺長</param>
	/// <returns>慣性テンソル</returns>
	static Matrix4x4 CreateRectangular(
		float mass,
		const Vector3& sideLength);

};

