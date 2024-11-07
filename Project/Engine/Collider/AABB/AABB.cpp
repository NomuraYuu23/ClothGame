#include "AABB.h"

void AABB::Initialize(const Vector3& min, const Vector3& max, ColliderParentObject parentObject)
{

	Collider::Initialize(parentObject);

	min_ = min;
	max_ = max;

}

void AABB::worldTransformUpdate()
{

	worldTransform_.transform_.translate = Vector3::Multiply(0.5f, Vector3::Add(max_, min_));
	worldTransform_.transform_.scale = Vector3::Multiply(0.5f, Vector3::Subtract(max_, min_));
	worldTransform_.UpdateMatrix();

}
