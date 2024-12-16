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

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="parentObject">親オブジェクト</param>
	void Initialize(ColliderParentObject parentObject);

	/// <summary>
	/// ワールドトランスフォーム更新
	/// </summary>
	virtual void worldTransformUpdate() = 0;

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
	/// 親オブジェクトアクセッサ
	/// </summary>
	/// <returns></returns>
	ColliderParentObject GetParentObject() { return parentObject_; }
	void SetParentObject(ColliderParentObject parentObject) { parentObject_ = parentObject; }

public: // デバッグ

	// ワールドトランスフォーム
	WorldTransform worldTransform_;

protected: // メンバ変数

	//衝突属性(自分)
	uint32_t collisionAttribute_ = 0xffffffff;

	// 衝突マスク(相手)
	uint32_t collisionMask_ = 0xffffffff;

	// 親オブジェクト
	ColliderParentObject parentObject_;

};

