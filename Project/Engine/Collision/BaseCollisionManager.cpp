#include "BaseCollisionManager.h"
#include "Collision.h"
#include "CollisionData.h"


void BaseCollisionManager::Initialize()
{

	ListClear();

}

void BaseCollisionManager::ListClear()
{
	colliders_.clear();
}

void BaseCollisionManager::ListRegister(ColliderShape* collider)
{

	colliders_.push_back(collider);

}

void BaseCollisionManager::CheakAllCollision()
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
