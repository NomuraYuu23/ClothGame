#include "Box.h"

void Box::Initialize(const Vector2& center, float width, float height, float rotation, ColliderParentObject2D parentObject)
{

	Collider2D::Initialize(parentObject);

	position_ = center;
	scale_.x = width;
	scale_.y = height;

	rotation_ = rotation;

}

void Box::Update(const Vector2& center, float width, float height, float rotation)
{

	position_ = center;
	scale_.x = width;
	scale_.y = height;

	rotation_ = rotation;

}
