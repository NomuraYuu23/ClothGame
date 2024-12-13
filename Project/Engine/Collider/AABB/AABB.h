#pragma once
#include "../Collider.h"

/// <summary>
/// AABB
/// </summary>
class AABB : 
	public Collider
{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="min">最小</param>
	/// <param name="max">最大</param>
	/// <param name="parentObject">親オブジェクト</param>
	void Initialize(const Vector3& min, const Vector3& max, ColliderParentObject parentObject);

	/// <summary>
	/// ワールドトランスフォーム更新
	/// </summary>
	void worldTransformUpdate() override;

public: // メンバ変数

	// 最小
	Vector3 min_;
	// 最大
	Vector3 max_;

};

