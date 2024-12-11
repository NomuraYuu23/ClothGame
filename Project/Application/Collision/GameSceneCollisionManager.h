#pragma once
#include "../../Engine/Collision/BaseCollisionManager.h"

/// <summary>
/// ゲームシーン用の衝突マネージャー
/// </summary>
class GameSceneCollisionManager :
    public BaseCollisionManager
{

private: // メンバ関数

	/// <summary>
	/// コライダー2つの衝突判定と応答
	/// </summary>
	/// <param name="colliderA">コライダーA</param>
	/// <param name="colliderB">コライダーB</param>
	void CheckCollisionPair(ColliderShape* colliderA, ColliderShape* colliderB) override;

};

