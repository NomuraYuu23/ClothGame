#include "Vector2.h"

Vector2 Vector2::Add(const Vector2& v1, const Vector2& v2)
{

	Vector2 result;

	result = { v1.x + v2.x, v1.y + v2.y };

	return result;

}

Vector2 Vector2::Subtract(const Vector2& v1, const Vector2& v2)
{

	Vector2 result;

	result = { v1.x - v2.x, v1.y - v2.y };

	return result;

}

Vector2 Vector2::Multiply(const Vector2& v, float scalar)
{
	
	Vector2 result;

	result = { scalar * v.x, scalar * v.y };

	return result;

}

float Vector2::Dot(const Vector2& v1, const Vector2& v2)
{

	float result;

	result = v1.x * v2.x + v1.y * v2.y;

	return result;

}

float Vector2::Length(const Vector2& v)
{
	
	float result;

	result = sqrtf(powf(v.x, 2.0f) + powf(v.y, 2.0f));

	return result;

}

Vector2 Vector2::Normalize(const Vector2& v)
{

	Vector2 result = {0.0f,0.0f};

	float num = Length(v);

	if (num != 0.0f) {
		result.x = v.x / num;
		result.y = v.y / num;
	}

	return result;

}

float Vector2::Cross(const Vector2& v1, const Vector2& v2)
{
	
	float result;

	result = v1.x * v2.y - v1.y * v2.x;

	return result;

}

Vector2 Vector2::operator+(const Vector2& v)
{

	Vector2 result = *this;

	result = Add(result, v);

	return result;

}

void Vector2::operator+=(const Vector2& v)
{
	
	Vector2 result = *this;

	result = Add(result, v);

	*this = result;

}

Vector2 Vector2::operator-(const Vector2& v)
{

	Vector2 result = *this;

	result = Subtract(result, v);

	return result;

}

void Vector2::operator-=(const Vector2& v)
{

	Vector2 result = *this;

	result = Subtract(result, v);

	*this = result;

}

Vector2 Vector2::operator*(float v)
{

	Vector2 result = *this;

	result = Multiply(result, v);

	return result;

}

void Vector2::operator*=(float v)
{

	Vector2 result = *this;

	result = Multiply(result, v);

	*this = result;

}
