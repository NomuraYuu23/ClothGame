#include "GameSceneCollisionManager.h"
#include "../../Engine/Collision/Collision.h"

#include "../Object/Player/Player.h"
#include "../Object/Enemy/BaseEnemy.h"
#include "../Object/GroundBlock/GroundBlock.h"
#include "../Object/ClothGate/BaseClothGate.h"

void GameSceneCollisionManager::CheckCollisionPair(ColliderShape* colliderA, ColliderShape* colliderB)
{

	std::visit([](auto& a, auto& b) {
		// 衝突フィルタリング

		if (!(a.GetCollisionAttribute() & b.GetCollisionMask()) ||
			!(b.GetCollisionAttribute() & a.GetCollisionMask())) {
			return;
		}
		Vector3 p1 = {};
		Vector3 p2 = {};
		float t1 = 0.0f;
		float t2 = 0.0f;
		float pushBackDist = 0.0f;
		if (Collision::IsCollision(a, b, p1, p2, t1, t2, pushBackDist)) {
			// 衝突処理
			std::visit([=](const auto& x, const auto& y) {
				CollisionData collisionData = { p1, t1, pushBackDist ,p2 };
				x->OnCollision(y, collisionData);
				collisionData = { p2, t2, pushBackDist ,p1 };
				y->OnCollision(x, collisionData);
				}, a.GetParentObject(), b.GetParentObject());
		}
		}, *colliderA, *colliderB);

}
