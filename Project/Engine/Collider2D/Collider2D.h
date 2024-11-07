#pragma once
#include "../../Application/Collider2D/ColliderParentObject2D.h"
#include <cstdint>
#include "../Math/Vector/Vector2.h"

class Collider2D
{

public: // メンバ関数

	void Initialize(ColliderParentObject2D parentObject);

public: //メンバ関数 (衝突)

	// 衝突属性(自分)を取得
	uint32_t GetCollisionAttribute() { return collisionAttribute_; }
	// 衝突属性(自分)を設定
	void SetCollisionAttribute(uint32_t collisionAttribute) { collisionAttribute_ = collisionAttribute; }
	// 衝突マスク(相手)を取得
	uint32_t GetCollisionMask() { return collisionMask_; }
	// 衝突マスク(相手)を設定
	void SetCollisionMask(uint32_t collisionMask) { collisionMask_ = collisionMask; }

	ColliderParentObject2D GetParentObject() { return parentObject_; }
public:
	// デバッグ用
	Vector2 position_{};
	Vector2 scale_{};

protected:

	//衝突属性(自分)
	uint32_t collisionAttribute_ = 0xffffffff;

	// 衝突マスク(相手)
	uint32_t collisionMask_ = 0xffffffff;

	// 親オブジェクト
	ColliderParentObject2D parentObject_;

};

