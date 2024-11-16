#include "Ground.h"

void Ground::Initialize(LevelData::MeshData* data)
{
	
	MeshObject::Initialize(data);

	material_->SetEnableLighting(BlinnPhongReflection);

	// コライダー
	OBB obb = std::get<OBB>(*collider_.get());
	obb.SetParentObject(this);
	//obb.SetCollisionAttribute(collisionAttribute_);
	//obb.SetCollisionMask(collisionMask_);
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

