#pragma once
#include "../Collider.h"
class AABB : public Collider
{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="center">センター</param>
	/// <param name="radius">半径</param>
	void Initialize(const Vector3& min, const Vector3& max, ColliderParentObject parentObject);

	/// <summary>
	/// ワールドトランスフォーム更新
	/// </summary>
	void worldTransformUpdate() override;

public: // メンバ変数

	Vector3 min_; // 最小
	Vector3 max_; // 最大

};

