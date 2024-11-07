#pragma once
#include "../Math/Vector/Vector2.h"
#include "../Math/Vector/Vector3.h"
#include <functional>
#include "../3D/Transform/WorldTransform.h"
#include "../../Application/Collider/ColliderParentObject.h"

/// <summary>
/// コライダー
/// </summary>
class Collider
{

public: // メンバ関数

	void Initialize(ColliderParentObject parentObject);

	virtual void worldTransformUpdate() = 0;

public: //メンバ関数 (衝突)

	// 衝突属性(自分)を取得
	uint32_t GetCollisionAttribute() { return collisionAttribute_; }
	// 衝突属性(自分)を設定
	void SetCollisionAttribute(uint32_t collisionAttribute) { collisionAttribute_ = collisionAttribute; }
	// 衝突マスク(相手)を取得
	uint32_t GetCollisionMask() { return collisionMask_; }
	// 衝突マスク(相手)を設定
	void SetCollisionMask(uint32_t collisionMask) { collisionMask_ = collisionMask; }
	
	ColliderParentObject GetParentObject() { return parentObject_; }
	void SetParentObject(ColliderParentObject parentObject) { parentObject_ = parentObject; }

public:
	// ワールドトランスフォーム
	WorldTransform worldTransform_;
protected:

	//衝突属性(自分)
	uint32_t collisionAttribute_ = 0xffffffff;

	// 衝突マスク(相手)
	uint32_t collisionMask_ = 0xffffffff;

	// 親オブジェクト
	ColliderParentObject parentObject_;

};

