#pragma once
#include <variant>
#include "../../Engine/Collision/CollisionData.h"

class ParentNullObject;
class Player;
class Ground;
class BaseEnemy;

using ColliderParentObject = std::variant<ParentNullObject*, Player*, Ground*, BaseEnemy*>;

// 親がないもの用
class ParentNullObject {

public:

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="colliderPartner"></param>
	/// <param name="collisionData"></param>
	void OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData) {}

};
