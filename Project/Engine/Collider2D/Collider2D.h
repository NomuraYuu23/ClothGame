#pragma once
#include "../../Application/Collider2D/ColliderParentObject2D.h"
#include <cstdint>
#include "../Math/Vector/Vector2.h"

/// <summary>
/// コライダー2D
/// </summary>
class Collider2D
{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="parentObject">親オブジェクト</param>
	void Initialize(ColliderParentObject2D parentObject);

public: //メンバ関数 (衝突)

	/// <summary>
	/// 衝突属性(自分)を取得
	/// </summary>
	/// <returns></returns>
	uint32_t GetCollisionAttribute() { return collisionAttribute_; }
	/// <summary>
	/// 衝突属性(自分)を設定
	/// </summary>
	/// <param name="collisionAttribute"></param>
	void SetCollisionAttribute(uint32_t collisionAttribute) { collisionAttribute_ = collisionAttribute; }
	/// <summary>
	/// 衝突マスク(相手)を取得
	/// </summary>
	/// <returns></returns>
	uint32_t GetCollisionMask() { return collisionMask_; }
	/// <summary>
	/// 衝突マスク(相手)を設定
	/// </summary>
	/// <param name="collisionMask"></param>
	void SetCollisionMask(uint32_t collisionMask) { collisionMask_ = collisionMask; }

	/// <summary>
	/// 親オブジェクト取得
	/// </summary>
	/// <returns></returns>
	ColliderParentObject2D GetParentObject() { return parentObject_; }

public: // デバッグ用 メンバ変数

	//位置
	Vector2 position_{};
	
	// 大きさ
	Vector2 scale_{};

protected: // メンバ変数

	//衝突属性(自分)
	uint32_t collisionAttribute_ = 0xffffffff;

	// 衝突マスク(相手)
	uint32_t collisionMask_ = 0xffffffff;

	// 親オブジェクト
	ColliderParentObject2D parentObject_;

};

