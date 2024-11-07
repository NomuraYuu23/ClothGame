#pragma once
#include "../Collider.h"
#include "../../Line/Segment.h"
class Capsule : public Collider
{

public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="center">センター</param>
	/// <param name="radius">半径</param>
	void Initialize(const Segment& segment, float radius, ColliderParentObject parentObject);

	/// <summary>
	/// ワールドトランスフォーム更新
	/// </summary>
	void worldTransformUpdate() override;

public:

	Segment segment_;
	float radius_;

};

