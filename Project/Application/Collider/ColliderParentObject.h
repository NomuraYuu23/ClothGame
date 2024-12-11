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

// variantでまとめる
using ColliderParentObject = std::variant<ParentNullObject*, Player*, BaseEnemy*, GroundBlock*>;

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
	void OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData) {}

};
