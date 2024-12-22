#pragma once
#include <variant>
#include "../../Engine/Collision/CollisionData.h"

/// <summary>
/// 衝突時に「OnCollision」を呼ぶための設定
/// </summary>

// 前方宣言
class ParentNullObject;
class Player;
class BaseEnemy;
class GroundBlock;
class BaseClothGate;

// variantでまとめる
using ColliderParentObject = std::variant<ParentNullObject*, Player*, BaseEnemy*, GroundBlock*, BaseClothGate*>;

/// <summary>
/// 親がないもの用
/// </summary>
class ParentNullObject {

public:

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="colliderPartner"></param>
	/// <param name="collisionData"></param>
	void OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData) {
		// 警告回避
		colliderPartner;
		collisionData;
	}

};
