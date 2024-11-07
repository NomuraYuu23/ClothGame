#include "Quaternion.h"
#include <cmath>

Quaternion Quaternion::Add(const Quaternion& q0, const Quaternion& q1)
{

	Quaternion result = {};

	result.x = q0.x + q1.x;
	result.y = q0.y + q1.y;
	result.z = q0.z + q1.z;
	result.w = q0.w + q1.w;

	return result;
}

Quaternion Quaternion::Subtract(const Quaternion& q0, const Quaternion& q1)
{

	Quaternion result = {};

	result.x = q0.x - q1.x;
	result.y = q0.y - q1.y;
	result.z = q0.z - q1.z;
	result.w = q0.w - q1.w;

	return result;

}

Quaternion Quaternion::Multiply(const Quaternion& lhs, const Quaternion& rhs)
{

	Quaternion result = {};

	Vector3 q = { lhs.x, lhs.y, lhs.z, };
	Vector3 r = { rhs.x, rhs.y, rhs.z, };
	Vector3 resultVector = Vector3::Add(Vector3::Cross(q, r), Vector3::Add(Vector3::Multiply(rhs.w, q), Vector3::Multiply(lhs.w, r)));

	result.x = resultVector.x;
	result.y = resultVector.y;
	result.z = resultVector.z;
	result.w = lhs.w * rhs.w - Vector3::Dot(q, r);

	return result;
}

Quaternion Quaternion::Multiply(const Quaternion& q, float s)
{

	Quaternion result = {};
	result.x = q.x * s;
	result.y = q.y * s;
	result.z = q.z * s;
	result.w = q.w * s;

	return result;
}

Quaternion Quaternion::Multiply(float s, const Quaternion& q)
{
	return Multiply(q, s);
}

Quaternion Quaternion::IdentityQuaternion()
{
	Quaternion result = { 0.0f,0.0f,0.0f,1.0f };
	return result;
}

Quaternion Quaternion::Conjugate(const Quaternion& quaternion)
{
	Quaternion result = { -quaternion.x, -quaternion.y, -quaternion.z, quaternion.w };
	return result;
}

float Quaternion::Norm(const Quaternion& quaternion)
{
	float result = sqrtf(quaternion.x * quaternion.x +
		quaternion.y * quaternion.y +
		quaternion.z * quaternion.z +
		quaternion.w * quaternion.w);

	return result;
}

Quaternion Quaternion::Normalize(const Quaternion& quaternion)
{
	Quaternion result = {};
	float norm = Norm(quaternion);

	if (quaternion.x != 0.0) {
		result.x = quaternion.x / norm;
	}
	else {
		result.x = 0.0f;
	}

	if (quaternion.y != 0.0) {
		result.y = quaternion.y / norm;
	}
	else {
		result.y = 0.0f;
	}

	if (quaternion.z != 0.0) {
		result.z = quaternion.z / norm;
	}
	else {
		result.z = 0.0f;
	}

	if (quaternion.w != 0.0) {
		result.w = quaternion.w / norm;
	}
	else {
		result.w = 0.0f;
	}

	return result;

}

Quaternion Quaternion::Inverse(const Quaternion& quaternion)
{

	Quaternion result = {};
	Quaternion conjugate = Conjugate(quaternion);

	float norm = Norm(quaternion);

	if (norm != 0.0f) {
		result.x = conjugate.x / (norm * norm);
		result.y = conjugate.y / (norm * norm);
		result.z = conjugate.z / (norm * norm);
		result.w = conjugate.w / (norm * norm);
	}

	return result;
}

Quaternion Quaternion::MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle)
{

	Quaternion result = { };
	Vector3 vector = Vector3::Multiply(std::sinf(angle / 2.0f), axis);
	result = { vector.x, vector.y, vector.z, std::cosf(angle / 2.0f) };

	return result;

}

Vector3 Quaternion::RotateVector(const Vector3& vector, const Quaternion& quaternion)
{

	Vector3 result = {};

	Quaternion r = { vector.x, vector.y, vector.z, 0.0f };
	Quaternion conjugate = Conjugate(quaternion);
	Quaternion newQuaternion = Multiply(Multiply(quaternion, r), conjugate);

	result.x = newQuaternion.x;
	result.y = newQuaternion.y;
	result.z = newQuaternion.z;

	return result;
}

Matrix4x4 Quaternion::MakeRotateMatrix(const Quaternion& quaternion)
{

	Matrix4x4 result = {};
	float x = quaternion.x;
	float y = quaternion.y;
	float z = quaternion.z;
	float w = quaternion.w;

	result.m[0][0] = std::powf(w, 2.0f) + std::powf(x, 2.0f) - std::powf(y, 2.0f) - std::powf(z, 2.0f);
	result.m[0][1] = 2 * (x * y + w * z);
	result.m[0][2] = 2 * (x * z - w * y);
	result.m[0][3] = 0.0f;
	result.m[1][0] = 2 * (x * y - w * z);
	result.m[1][1] = std::powf(w, 2.0f) - std::powf(x, 2.0f) + std::powf(y, 2.0f) - std::powf(z, 2.0f);
	result.m[1][2] = 2 * (y * z + w * x);
	result.m[1][3] = 0.0f;
	result.m[2][0] = 2 * (x * z + w * y);
	result.m[2][1] = 2 * (y * z - w * x);
	result.m[2][2] = std::powf(w, 2.0f) - std::powf(x, 2.0f) - std::powf(y, 2.0f) + std::powf(z, 2.0f);
	result.m[2][3] = 0.0f;
	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;

	return result;

}

Quaternion Quaternion::Slerp(const Quaternion& q0, const Quaternion& q1, float t)
{

	const float kEpsilon = 0.0005f;
	float dot = q0.x * q1.x + q0.y * q1.y + q0.z * q1.z + q0.w * q1.w; // q0とq1の内積

	Quaternion use_q0 = q0;

	if (dot < 0.0f) {
		use_q0 = Multiply(q0, -1.0f);
		dot = -dot;
	}

	if (dot >= 1.0f - kEpsilon) {
		Quaternion result = Add(Multiply(use_q0, 1.0f - t), Multiply(q1, t));
		return result;
	}

	// なす角を求める
	float theta = std::acosf(dot);

	// thetaとsinを使って補間係数scale0,scale1を求める
	float scale0 = std::sinf((1.0f - t) * theta) / std::sinf(theta);
	float scale1 = std::sinf(t * theta) / std::sinf(theta);

	Quaternion result = Add(Multiply(use_q0, scale0), Multiply(q1, scale1));

	return result;

}

Quaternion Quaternion::DirectionToDirection(const Vector3& v0, const Vector3& v1)
{

	// n
	Vector3 n = Vector3::Cross(v0, v1);

	// cosTheta
	float cosTheta = Vector3::Dot(v0, v1);

	// なす角を求める
	float theta = acosf(cosTheta);

	Quaternion result = {
		n.x * sinf(theta / 2.0f),
		n.y * sinf(theta / 2.0f),
		n.z * sinf(theta / 2.0f),
		cosf(theta)
	};

	return result;

}

Quaternion Quaternion::operator+(const Quaternion& v)
{

	Quaternion result = *this;

	result = Add(result, v);

	return result;

}

void Quaternion::operator+=(const Quaternion& v)
{

	Quaternion result = *this;

	result = Add(result, v);

	*this = result;

}

Quaternion Quaternion::operator-(const Quaternion& v)
{
	
	Quaternion result = *this;

	result = Subtract(result, v);

	return result;

}

void Quaternion::operator-=(const Quaternion& v)
{

	Quaternion result = *this;

	result = Subtract(result, v);

	*this = result;

}

Quaternion Quaternion::operator*(float v)
{

	Quaternion result = *this;

	result = Multiply(result, v);

	return result;

}

void Quaternion::operator*=(float v)
{

	Quaternion result = *this;

	result = Multiply(result, v);

	*this = result;

}

Quaternion Quaternion::operator*(const Quaternion& v)
{

	Quaternion result = *this;

	result = Multiply(result, v);

	return result;

}

void Quaternion::operator*=(const Quaternion& v)
{

	Quaternion result = *this;

	result = Multiply(result, v);

	*this = result;

}
