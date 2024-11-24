#include "GroundBlock.h"
#include "../../Collider/CollisionConfig.h"

void GroundBlock::Initialize(LevelData::MeshData* data)
{

	MeshObject::Initialize(data);

	// マテリアル
	material_->SetEnableLighting(BlinnPhongReflection);
	const EulerTransform uvTransform = {
	worldTransform_.transform_.scale.x / 4.0f,
	worldTransform_.transform_.scale.z / 4.0f,
	1.0f,
	0.0f,0.0f,0.0f,
	0.0f,0.0f,0.0f
	};
	material_->SetUvTransform(uvTransform);

	// 衝突マスク
	collisionAttribute_ = kCollisionAttributeGround;
	collisionMask_ -= kCollisionAttributeGround;

	// コライダー
	OBB obb = std::get<OBB>(*collider_.get());
	obb.SetParentObject(this);
	obb.SetCollisionAttribute(collisionAttribute_);
	obb.SetCollisionMask(collisionMask_);
	ColliderShape* colliderShape = new ColliderShape();
	*colliderShape = obb;
	collider_.reset(colliderShape);

}

void GroundBlock::Update()
{

	OBB obb = std::get<OBB>(*collider_.get());

	obb.center_ = worldTransform_.GetWorldPosition();

	obb.SetOtientatuons(worldTransform_.rotateMatrix_);

	ColliderShape* colliderShape = new ColliderShape();

	*colliderShape = obb;

	collider_.reset(colliderShape);

}

void GroundBlock::OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{
}
