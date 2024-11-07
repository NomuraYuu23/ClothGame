#include "InertiaTensor.h"

Matrix4x4 InertiaTensor::CreateRectangular(float mass, const Vector3& sideLength)
{

    Matrix4x4 result;
    result = Matrix4x4::MakeIdentity4x4();

    result.m[0][0] = std::powf(sideLength.y, 2.0f) + std::powf(sideLength.z, 2.0f);
    result.m[1][1] = std::powf(sideLength.z, 2.0f) + std::powf(sideLength.x, 2.0f);
    result.m[2][2] = std::powf(sideLength.x, 2.0f) + std::powf(sideLength.y, 2.0f);

    result *= 1.0f / 12.0f * mass;

    return result;

}
