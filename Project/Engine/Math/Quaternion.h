#pragma once
#include "Vector/Vector3.h"
#include "Matrix/Matrix4x4.h"

class Quaternion
{

public:

	float x;
	float y;
	float z;
	float w;

public:

	static Quaternion Add(const Quaternion& q0, const Quaternion& q1);

    static Quaternion Subtract(const Quaternion& q0, const Quaternion& q1);

	static Quaternion Multiply(const Quaternion& lhs, const Quaternion& rhs);

	static Quaternion Multiply(const Quaternion& q, float s);

	static Quaternion Multiply(float s, const Quaternion& q);

	static Quaternion IdentityQuaternion();

	static Quaternion Conjugate(const Quaternion& quaternion);

	static float Norm(const Quaternion& quaternion);

	static Quaternion Normalize(const Quaternion& quaternion);

	static Quaternion Inverse(const Quaternion& quaternion);

	static Quaternion MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle);

	static Vector3 RotateVector(const Vector3& vector, const Quaternion& quaternion);

	static Matrix4x4 MakeRotateMatrix(const Quaternion& quaternion);

	static Quaternion Slerp(const Quaternion& q0, const Quaternion& q1, float t);

    static Quaternion DirectionToDirection(const Vector3& v0, const Vector3& v1);

public: // オーバーロード

    /// <summary>
    /// +
    /// </summary>
    /// <param name="v"></param>
    /// <returns></returns>
    Quaternion operator+(const Quaternion& v);

    /// <summary>
    /// +=
    /// </summary>
    /// <param name="v"></param>
    void operator+=(const Quaternion& v);

    /// <summary>
    /// -
    /// </summary>
    /// <param name="v"></param>
    /// <returns></returns>
    Quaternion operator-(const Quaternion& v);

    /// <summary>
    /// -=
    /// </summary>
    /// <param name="v"></param>
    void operator-=(const Quaternion& v);

    /// <summary>
    /// *
    /// </summary>
    /// <param name="v"></param>
    /// <returns></returns>
    Quaternion operator*(float v);

    /// <summary>
    /// *=
    /// </summary>
    /// <param name="v"></param>
    void operator*=(float v);

    /// <summary>
    /// *
    /// </summary>
    /// <param name="v"></param>
    /// <returns></returns>
    Quaternion operator*(const Quaternion& v);

    /// <summary>
    /// *=
    /// </summary>
    /// <param name="v"></param>
    void operator*=(const Quaternion& v);

};
