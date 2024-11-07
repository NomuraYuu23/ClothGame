#include "Segment2D.h"

void Segment2D::Initialize(const Vector2& origin, const Vector2& length, ColliderParentObject2D parentObject)
{

	Collider2D::Initialize(parentObject);

	origin_ = origin;

	length_ = length;

}

void Segment2D::Update(const Vector2& origin, const Vector2& length)
{

	origin_ = origin;

	length_ = length;

}
