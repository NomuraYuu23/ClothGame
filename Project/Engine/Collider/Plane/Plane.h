#pragma once
#include "../Collider.h"

/// <summary>
/// 衝突オブジェクト 平面
/// </summary>
class Plane :
    public Collider
{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="center">センター</param>
	/// <param name="radius">半径</param>
	void Initialize(const Vector3& normal, float distance, ColliderParentObject parentObject);

	/// <summary>
	/// ワールドトランスフォーム更新
	/// </summary>
	void worldTransformUpdate() override;

public: // メンバ変数

	// 法線
	Vector3 normal_;
	// 距離
	float distance_;

};
