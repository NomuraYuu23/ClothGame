#pragma once
#include "Collider2D.h"
/// <summary>
/// 2D衝突オブジェクト 線分
/// </summary>
class Segment2D : 
	public Collider2D
{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="origin">原点</param>
	/// <param name="diff">長さ</param>
	/// <param name="parentObject">親オブジェクト</param>
	void Initialize(const Vector2& origin, const Vector2& length, ColliderParentObject2D parentObject);

	/// <summary>
	/// 簡単更新
	/// </summary>
	/// <param name="origin">原点</param>
	/// <param name="diff">長さ</param>
	void Update(const Vector2& origin, const Vector2& length);

public: // メンバ変数

	// 原点
	Vector2 origin_{};
	// 長さ
	Vector2 length_{};

};

