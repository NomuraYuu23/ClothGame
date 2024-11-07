#include "Line.h"
#include <cmath>
#include <algorithm>

void Line::Initialize(const Vector3& origin, const Vector3& diff)
{

	origin_ = origin;
	diff_ = diff;

}

Vector3 Line::ClosestPoint(const Vector3& point, const Line& line)
{
	float t = Vector3::Dot(Vector3::Subtract(point, line.origin_), line.diff_) / std::powf(Vector3::Length(line.diff_), 2.0f);
	t = std::clamp(t, 1.0f, 0.0f);
	return Vector3::Add(line.origin_, Vector3::Multiply(t, line.diff_));
}
