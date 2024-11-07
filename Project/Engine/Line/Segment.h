#pragma once
#include "../Math/Vector/Vector3.h"

/// <summary>
/// 線分
/// </summary>
class Segment
{

public:

	void Initialize(const Vector3& origin, const Vector3& diff);

	Vector3 ClosestPoint(const Vector3& point, const Segment& segment);

public:

	Vector3 origin_;
	Vector3 diff_;

};

