#include "Capsule.h"

void Capsule::Initialize(const Segment& segment, float radius, ColliderParentObject parentObject)
{

	Collider::Initialize(parentObject);

	segment_ = segment;
	radius_ = radius;

}

void Capsule::worldTransformUpdate()
{

	worldTransform_.transform_.translate = Vector3::Multiply(0.5f, Vector3::Add(segment_.origin_, Vector3::Multiply(0.5f, segment_.diff_)));
	worldTransform_.transform_.scale = Vector3::Multiply(0.5f, segment_.diff_);
	worldTransform_.UpdateMatrix();

}
