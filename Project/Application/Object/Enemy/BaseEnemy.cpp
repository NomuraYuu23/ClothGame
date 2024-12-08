#include "BaseEnemy.h"

BaseEnemy::BaseEnemy()
{
}

BaseEnemy::~BaseEnemy()
{
}

void BaseEnemy::Initialize(LevelData::MeshData* data)
{

	// メッシュオブジェクトの初期化
	MeshObject::Initialize(data);

	// 衝突マスク
	collisionAttribute_ = kCollisionAttributeEnemy;
	collisionMask_ -= kCollisionAttributeEnemy;

	// コライダー
	OBB obb = std::get<OBB>(*collider_.get());
	obb.SetParentObject(this);
	obb.SetCollisionAttribute(collisionAttribute_);
	obb.SetCollisionMask(collisionMask_);
	ColliderShape* colliderShape = new ColliderShape();
	*colliderShape = obb;
	collider_.reset(colliderShape);

	// プレイヤーと衝突するか
	conflictWithPlayer_ = true;

}

void BaseEnemy::ColliderUpdate()
{

	OBB obb = std::get<OBB>(*collider_.get());

	obb.center_ = worldTransform_.GetWorldPosition();

	obb.SetOtientatuons(worldTransform_.rotateMatrix_);

	ColliderShape* colliderShape = new ColliderShape();

	*colliderShape = obb;

	collider_.reset(colliderShape);

}
