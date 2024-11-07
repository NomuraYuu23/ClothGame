#include "Collider2D.h"

void Collider2D::Initialize(ColliderParentObject2D parentObject)
{

	position_ = { 0.0f, 0.0f };
	scale_ = { 1.0f, 1.0f };

	parentObject_ = parentObject;

}
