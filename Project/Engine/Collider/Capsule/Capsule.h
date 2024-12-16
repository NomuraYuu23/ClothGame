#pragma once
#include "../Collider.h"
#include "../../Line/Segment.h"

/// <summary>
/// 衝突オブジェクト カプセル
/// </summary>
class Capsule : 
	public Collider
{

public: // メンバ関数

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

public: // メンバ変数

	// 線分
	Segment segment_;
	// 半径
	float radius_;

};

