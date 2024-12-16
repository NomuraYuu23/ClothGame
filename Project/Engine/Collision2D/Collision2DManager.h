#pragma once

#include "../Collider2D/Collider2D.h"
#include "../Collider2D/ColliderShape2D.h"

/// <summary>
/// 2D衝突マネージャー
/// </summary>
class Collision2DManager
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
	/// <param name="collider"></param>
	void ListRegister(ColliderShape2D collider);

	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void CheakAllCollision();

	/// <summary>
	/// コライダー取得
	/// </summary>
	/// <returns></returns>
	std::list<ColliderShape2D> GetColliders() { return colliders_; }

private: // メンバ関数

	/// <summary>
	/// コライダー2つの衝突判定と応答
	/// </summary>
	/// <param name="colliderA">コライダーA</param>
	/// <param name="colliderB">コライダーB</param>
	void CheckCollisionPair(ColliderShape2D colliderA, ColliderShape2D colliderB);

private: // メンバ変数

	// コライダーリスト
	std::list<ColliderShape2D> colliders_;

};

