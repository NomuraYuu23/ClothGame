#pragma once
#include "../Collider.h"

/// <summary>
/// 衝突オブジェクト 球
/// </summary>
class Sphere : 
	public Collider
{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="center">センター</param>
	/// <param name="radius">半径</param>
	void Initialize(const Vector3& center, float radius, ColliderParentObject parentObject);

	/// <summary>
	/// ワールドトランスフォーム更新
	/// </summary>
	void worldTransformUpdate() override;

public: // メンバ変数

	// 中心点
	Vector3 center_;
	// 半径
	float radius_;

};

