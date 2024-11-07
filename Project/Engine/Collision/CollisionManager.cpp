#include "CollisionManager.h"
#include "Collision.h"
#include "CollisionData.h"


void CollisionManager::Initialize()
{

	ListClear();

}

void CollisionManager::ListClear()
{
	colliders_.clear();
}

void CollisionManager::ListRegister(ColliderShape* collider)
{

	colliders_.push_back(collider);

}

void CollisionManager::CheakAllCollision()
{

	// リスト内のペアを総当たり
	std::list<ColliderShape*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA) {
		// イテレータAからコライダーAを取得する
		ColliderShape* colliderA = *itrA;
		// イテレータBはイテレータAの次の要素から回す(重複判定を回避)
		std::list<ColliderShape*>::iterator itrB = itrA;
		itrB++;

		for (; itrB != colliders_.end(); ++itrB) {
			// イテレータBからコライダーBを取得する
			ColliderShape* colliderB = *itrB;

			// ペアの当たり判定
			CheckCollisionPair(colliderA, colliderB);
		}
	}

}

void CollisionManager::CheckCollisionPair(ColliderShape* colliderA, ColliderShape* colliderB)
{

	std::visit([](auto& a, auto& b) {
		// 衝突フィルタリング

		if (!(a.GetCollisionAttribute() & b.GetCollisionMask()) ||
			!(b.GetCollisionAttribute() & a.GetCollisionMask())) {
			return ;
		}
		Vector3 p1 = {};
		Vector3 p2 = {};
		float t1 = 0.0f;
		float t2 = 0.0f;
		float pushBackDist = 0.0f;
		if (Collision::IsCollision(a, b, p1, p2, t1, t2, pushBackDist)) {
			// 衝突処理
			std::visit([=](const auto& x, const auto& y) {
				CollisionData collisionData = { p1, t1, pushBackDist ,p2};
				x->OnCollision(y, collisionData);
				collisionData = { p2, t2, pushBackDist ,p1};
				y->OnCollision(x, collisionData);
				}, a.GetParentObject(), b.GetParentObject());
		}
	}, *colliderA, *colliderB);

}
