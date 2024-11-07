#include "Velocity2DData.h"

void Velocity2DData::SetVelocity(const Vector3& velocity3D, const Matrix4x4& viewMatrix)
{

	Vector3 value = Matrix4x4::TransformNormal(velocity3D, viewMatrix);

	velocity = { value.x, value.y };

}
