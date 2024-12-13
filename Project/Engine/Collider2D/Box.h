#pragma once
#include "Collider2D.h"

/// <summary>
/// 2D衝突オブジェクト 四角形
/// </summary>
class Box : 
	public Collider2D
{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="center">中心/param>
	/// <param name="width">幅(直径)</param>
	/// <param name="height">高さ(直径)</param>
	/// <param name="parentObject">親オブジェクト</param>
	void Initialize(const Vector2& center, float width, float height, float rotation, ColliderParentObject2D parentObject);

	/// <summary>
	/// 簡単更新
	/// </summary>
	/// <param name="center">中心</param>
	/// <param name="width">幅(直径)</param>
	/// <param name="height">高さ(直径)</param>
	void Update(const Vector2& center,float width, float height, float rotation);

public: // メンバ変数

	// 回転
	float rotation_ = 0.0f;

};
