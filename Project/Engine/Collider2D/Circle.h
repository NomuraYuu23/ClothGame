#pragma once
#include "Collider2D.h"

/// <summary>
/// 2D衝突オブジェクト 円
/// </summary>
class Circle : 
	public Collider2D
{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="center">中心/param>
	/// <param name="radius">半径</param>
	/// <param name="parentObject">親オブジェクト</param>
	void Initialize(const Vector2& center, float radius, ColliderParentObject2D parentObject);

	/// <summary>
	/// 簡単更新
	/// </summary>
	/// <param name="center">中心</param>
	/// <param name="radius">半径</param>
	void Update(const Vector2& center, float radius);

public: // メンバ変数

	// 半径
	float radius_ = 0.0f;

};

