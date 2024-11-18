#include "Ground.h"
#include "../../Collider/CollisionConfig.h"

void Ground::Initialize(LevelData::MeshData* data)
{
	
	MeshObject::Initialize(data);

	// マテリアル
	material_->SetEnableLighting(BlinnPhongReflection);
	const EulerTransform uvTransform = {
		2.0f, 50.0f,1.0f,
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

void Ground::Update()
{
}

void Ground::OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{
}

