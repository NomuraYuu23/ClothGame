#pragma once

#include "../Collider2D/Collider2D.h"
#include "../Collider2D/ColliderShape2D.h"

class Collision2DManager
{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	// リストのクリア
	void ListClear();

	// リスト登録
	void ListRegister(ColliderShape2D collider);

	// 衝突判定と応答
	void CheakAllCollision();

	std::list<ColliderShape2D> GetColliders() { return colliders_; }

private:

	// コライダーリスト
	std::list<ColliderShape2D> colliders_;

	// コライダー2つの衝突判定と応答
	void CheckCollisionPair(ColliderShape2D colliderA, ColliderShape2D colliderB);


};

