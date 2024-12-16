#pragma once
#include "../../Engine/Math/Vector/Vector3.h"
#include "../../Engine/Math/Matrix/Matrix4x4.h"
#include "../Collider/Collider.h"
#include "../Collider/ColliderShape.h"

/// <summary>
/// 衝突マネージャー
/// </summary>
class BaseCollisionManager
{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// リストのクリア
	/// </summary>
	void ListClear();

	/// <summary>
	/// リスト登録
	/// </summary>
	/// <param name="collider">コライダー</param>
	void ListRegister(ColliderShape* collider);

	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void CheakAllCollision();

protected: // メンバ変数

	/// <summary>
	/// コライダー2つの衝突判定と応答
	/// </summary>
	/// <param name="colliderA">コライダーA</param>
	/// <param name="colliderB">コライダーB</param>
	virtual void CheckCollisionPair(ColliderShape* colliderA, ColliderShape* colliderB) = 0;

protected: // メンバ変数

	// コライダーリスト
	std::list<ColliderShape*> colliders_;

};
