#pragma once
#include <variant>
#include "../../Engine/Collision/CollisionData.h"

class ParentNullObject;
class Player;
class Ground;

using ColliderParentObject = std::variant<ParentNullObject*, Player*, Ground*>;

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
