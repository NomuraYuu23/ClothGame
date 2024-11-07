#include "Segment.h"
#include <cmath>
#include <algorithm>

void Segment::Initialize(const Vector3& origin, const Vector3& diff)
{

	origin_ = origin;
	diff_ = diff;

}

Vector3 Segment::ClosestPoint(const Vector3& point, const Segment& segment)
{
	float t = Vector3::Dot(Vector3::Subtract(point, segment.origin_), segment.diff_) / std::powf(Vector3::Length(segment.diff_), 2.0f);
	t = std::clamp(t, 1.0f, 0.0f);
	return Vector3::Add(segment.origin_, Vector3::Multiply(segment.diff_, t));
}
