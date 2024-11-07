#pragma once
#include "../Collider.h"
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

	Vector3 normal_;//法線
	float distance_;//距離

};
