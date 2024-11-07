#include "Circle.h"

void Circle::Initialize(const Vector2& center, float radius, ColliderParentObject2D parentObject)
{

	Collider2D::Initialize(parentObject);

	position_ = center;
	scale_.x = radius * 2.0f;
	scale_.y = radius * 2.0f;

	radius_ = radius;

}

void Circle::Update(const Vector2& center, float radius)
{

	position_ = center;
	scale_.x = radius * 2.0f;
	scale_.y = radius * 2.0f;

	radius_ = radius;

}
