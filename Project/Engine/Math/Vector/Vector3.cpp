#include "Vector3.h"
#include <cmath>

//加算
Vector3 Vector3::Add(const Vector3& v1, const Vector3& v2) {

	Vector3 result;

	result = { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };

	return result;

}

//減算
Vector3 Vector3::Subtract(const Vector3& v1, const Vector3& v2) {

	Vector3 result;

	result = { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };

	return result;

}

//スカラー倍
Vector3 Vector3::Multiply(const Vector3& v, float scalar) {

	Vector3 result;

	result = { scalar * v.x, scalar * v.y, scalar * v.z };

	return result;

}

Vector3 Vector3::Multiply(float scalar, const Vector3& v)
{
	return Multiply(v, scalar);
}

//内積
float Vector3::Dot(const Vector3& v1, const Vector3& v2) {

	float result;

	result = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;

	return result;

}

//長さ（ノルム）
float Vector3::Length(const Vector3& v) {

	float result;

	result = std::sqrtf(Dot(v, v));

	return result;

}


//正規化
Vector3 Vector3::Normalize(const Vector3& v) {

	Vector3 result;
	float norm;

	norm = Length(v);

	if (v.x != 0.0) {
		result.x = v.x / norm;
	}
	else {
		result.x = 0.0f;
	}

	if (v.y != 0.0) {
		result.y = v.y / norm;
	}
	else {
		result.y = 0.0f;
	}


	if (v.z != 0.0) {
		result.z = v.z / norm;
	}
	else {
		result.z = 0.0f;
	}

	return result;

}

Vector3 Vector3::MaximumNormalize(const Vector3& v)
{

	Vector3 result = {0.0f,0.0f,0.0f};

	float norm = GetAbsMax(v);

	if (norm != 0.0f) {
		//正規化
		result.x = v.x / norm;
		result.y = v.y / norm;
		result.z = v.z / norm;
	}

	return result;

}

//クロス積
Vector3 Vector3::Cross(const Vector3& v1, const Vector3& v2) {

	Vector3 result = { v1.y * v2.z - v1.z * v2.y,v1.z * v2.x - v1.x * v2.z,v1.x * v2.y - v1.y * v2.x, };

	return result;

}

// 線形補間
Vector3 Vector3::Lerp(const Vector3& v1, const Vector3& v2, float t) {

	Vector3 result = Add(v1, Multiply(Subtract(v2, v1), t));

	return result;
}

// 3次スプライン曲線
Vector3 Vector3::CatmullRomSpline(
	const std::vector<Vector3>& controlPoints, const float& t) {

	Vector3 result;

	uint32_t point = int(float(controlPoints.size() - 1) * t);
	float t_ = float(controlPoints.size() - 1) * t - float(point);
	if (point == controlPoints.size() - 1) {
		point = int(float(controlPoints.size() - 1));
		t_ = 1.0f;
	}

	Vector3 p0, p1, p2, p3;

	if (point <= 0) {

		p0 = controlPoints[0];
		p1 = controlPoints[0];
		p2 = controlPoints[1];
		p3 = controlPoints[2];

	}
	else if (point >= controlPoints.size() - 2) {

		p0 = controlPoints[controlPoints.size() - 3];
		p1 = controlPoints[controlPoints.size() - 2];
		p2 = controlPoints[controlPoints.size() - 1];
		p3 = controlPoints[controlPoints.size() - 1];

	}
	else {

		p0 = controlPoints[--point];
		p1 = controlPoints[++point];
		p2 = controlPoints[++point];
		p3 = controlPoints[++point];
	}

	result.x = 1.0f / 2.0f *
		((-1.0f * p0.x + 3.0f * p1.x + -3.0f * p2.x + p3.x) * std::powf(t_, 3.0f) +
			(2.0f * p0.x + -5.0f * p1.x + 4.0f * p2.x + -1.0f * p3.x) * std::powf(t_, 2.0f) +
			(-1.0f * p0.x + p2.x) * t_ + 2.0f * p1.x);

	result.y = 1.0f / 2.0f *
		((-1.0f * p0.y + 3.0f * p1.y + -3.0f * p2.y + p3.y) * std::powf(t_, 3.0f) +
			(2.0f * p0.y + -5.0f * p1.y + 4.0f * p2.y + -1.0f * p3.y) * std::powf(t_, 2.0f) +
			(-1.0f * p0.y + p2.y) * t_ + 2.0f * p1.y);

	result.z = 1.0f / 2.0f *
		((-1.0f * p0.z + 3.0f * p1.z + -3.0f * p2.z + p3.z) * std::powf(t_, 3.0f) +
			(2.0f * p0.z + -5.0f * p1.z + 4.0f * p2.z + -1.0f * p3.z) * std::powf(t_, 2.0f) +
			(-1.0f * p0.z + p2.z) * t_ + 2.0f * p1.z);

	return result;
}

// 反射ベクトル
Vector3 Vector3::Reflect(const Vector3& input, const Vector3& normal) {

	return Subtract(input, Multiply(normal, 2.0f * Dot(input, normal)));

}

float Vector3::GetAbsMax(const Vector3& v)
{

	//絶対値を保存
	Vector3 abs;
	abs.x = std::fabsf(v.x);
	abs.y = std::fabsf(v.y);
	abs.z = std::fabsf(v.z);

	if (abs.x >= abs.y) {
		if (abs.x >= abs.z) {
			return abs.x;
		}
		else {
			return abs.z;
		}
	}
	else {
		if (abs.y >= abs.z) {
			return abs.y;
		}
		else {
			return abs.x;
		}
	}

	return 0.0f;
}

Vector3 Vector3::operator+(const Vector3& v)
{

	Vector3 result = *this;

	result = Add(result, v);

	return result;

}

void Vector3::operator+=(const Vector3& v)
{

	Vector3 result = *this;

	result = Add(result, v);

	*this = result;

}

Vector3 Vector3::operator-(const Vector3& v)
{

	Vector3 result = *this;

	result = Subtract(result, v);

	return result;

}

void Vector3::operator-=(const Vector3& v)
{
	
	Vector3 result = *this;

	result = Subtract(result, v);

	*this = result;

}

Vector3 Vector3::operator*(float v)
{

	Vector3 result = *this;

	result = Multiply(result, v);

	return result;

}

void Vector3::operator*=(float v)
{

	Vector3 result = *this;

	result = Multiply(result, v);

	*this = result;

}
