#include "Triangle.h"

void Triangle::Initialize(const std::array<Vector3, 3>& vertices, ColliderParentObject parentObject)
{

	Collider::Initialize(parentObject);

	SetVertices(vertices);

}

void Triangle::SetVertices(const std::array<Vector3, 3>& vertices)
{

	vertices_ = vertices;

}

void Triangle::worldTransformUpdate()
{
}
