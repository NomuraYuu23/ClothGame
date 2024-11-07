#include "ShortestDistance.h"
#include <cmath>
#include <algorithm>

ShortestDistance* ShortestDistance::GetInstance()
{
	static ShortestDistance instance;
	return &instance;
}

float ShortestDistance::PointLineDist(const Vector3& p, const Line& l, Vector3& h, float& t)
{

	float lenSqV = Vector3::Dot(l.diff_, l.diff_);
	t = 0.0f;
	if (lenSqV > 0.0f) {
		t = Vector3::Dot(l.diff_, Vector3::Subtract(p, l.origin_)) / lenSqV;
	}

	h = Vector3::Add(l.origin_, Vector3::Multiply(t, l.diff_));

	return Vector3::Length(Vector3::Subtract(h, p));

}

float ShortestDistance::PointSegmentDist(const Vector3& p, const Segment& seg, Vector3& h, float& t)
{

	const Vector3 e = Vector3::Add(seg.origin_, seg.diff_);

	// 垂線の長さ、垂線の足の座標およびtを算出
	float len = PointLineDist(p, Line(seg.origin_, Vector3::Subtract(e, seg.origin_)), h, t);

	if (Vector3::Dot(Vector3::Subtract(p, seg.origin_), Vector3::Subtract(e, seg.origin_)) <= 0.0f) {
		// 始点側の外側
		h = seg.origin_;
		return Vector3::Length(Vector3::Subtract(seg.origin_, p));
	}
	else if (Vector3::Dot(Vector3::Subtract(p, e), Vector3::Subtract(seg.origin_, e)) <= 0.0f) {
		// 終点側の外側
		h = e;
		return Vector3::Length(Vector3::Subtract(e, p));
	}

	return len;

}

float ShortestDistance::LineLineDist(const Line& l1, const Line& l2, Vector3& p1, Vector3& p2, float& t1, float& t2)
{

	// 2直線が平行?
	if (std::fabsf(Vector3::Dot(Vector3::Normalize(l1.diff_), Vector3::Normalize(l2.diff_))) == 1.0f) {

		float len = PointLineDist(l1.origin_, l2, p2, t2);
		p1 = l1.origin_;
		t1 = 0.0f;
		return len;

	}

	// 2直線はねじれ関係
	float dotV1V2 = Vector3::Dot(l1.diff_, l2.diff_);
	float dotV1V1 = Vector3::Dot(l1.diff_, l1.diff_);
	float dotV2V2 = Vector3::Dot(l2.diff_, l2.diff_);
	Vector3 VecL1L2 = Vector3::Subtract(l1.origin_, l2.origin_);

	t1 = (dotV1V2 * Vector3::Dot(l2.diff_, VecL1L2) - dotV2V2 * Vector3::Dot(l1.diff_, VecL1L2)) / (dotV1V1 * dotV2V2 - dotV1V2 * dotV1V2);
	p1 = Vector3::Add(Vector3::Multiply(1.0f - t1, l1.origin_), Vector3::Multiply(t1, Vector3::Add(l1.origin_, l1.diff_)));
	t2 = Vector3::Dot(l2.diff_, Vector3::Subtract(p1, l2.origin_)) / dotV2V2;
	p2 = Vector3::Add(Vector3::Multiply(1.0f - t2, l2.origin_), Vector3::Multiply(t2, Vector3::Add(l2.origin_, l2.diff_)));

	return Vector3::Length(Vector3::Subtract(p2, p1));

}

float ShortestDistance::SegmentSegmentDist(const Segment& seg1, const Segment& seg2, Vector3& p1, Vector3& p2, float& t1, float& t2)
{

	// S1が縮退している?
	if (Vector3::Length(Vector3::Normalize(seg1.diff_)) == 0.0f) {
		// s2も縮退か？
		if (Vector3::Length(Vector3::Normalize(seg2.diff_)) == 0.0f) {
			float len = Vector3::Length(Vector3::Subtract(seg2.origin_, seg1.origin_));
			p1 = seg1.origin_;
			p2 = seg2.origin_;
			t1 = 0.0f;
			t2 = 0.0f;
			return len;
		}
		else {
			float len = PointSegmentDist(seg1.origin_, seg2, p2, t2);
			p1 = seg1.origin_;
			t1 = 0.0f;
			t2 = std::clamp(t2, 0.0f, 1.0f);
			return len;
		}
	}

	// S2が縮退している?
	else if (Vector3::Length(Vector3::Normalize(seg2.diff_)) == 0.0f) {
		float len = PointSegmentDist(seg2.origin_, seg1, p1, t1);
		p2 = seg2.origin_;
		t2 = 0.0f;
		t1 = std::clamp(t1, 0.0f, 1.0f);
		return len;
	}

	// 線分同士

	// 2線分が平行?
	if (std::fabsf(Vector3::Dot(Vector3::Normalize(seg1.diff_), Vector3::Normalize(seg2.diff_))) == 1.0f) {
		float len = PointSegmentDist(seg1.origin_, seg2, p2, t2);
		p1 = seg1.origin_;
		t1 = 0.0f;
		if (0.0f <= t2 && t2 <= 1.0f) {
			return len;
		}
	}
	else {
		float len = LineLineDist(Line(seg1.origin_, seg1.diff_), Line(seg2.origin_, seg2.diff_), p1, p2, t1, t2);
		if (0.0f <= t1 && t1 <= 1.0f &&
			0.0f <= t2 && t2 <= 1.0f) {
			return len;
		}
	}

	// 垂線の足が外にある
	t1 = std::clamp(t1, 0.0f, 1.0f);
	p1 = Vector3::Add(Vector3::Multiply(1.0f - t1, seg1.origin_), Vector3::Multiply(t1, Vector3::Add(seg1.origin_, seg1.diff_)));
	float len = PointSegmentDist(p1, seg2, p2, t2);
	if (0.0f <= t2 && t2 <= 1.0f) {
		return len;
	}

	t2 = std::clamp(t2, 0.0f, 1.0f);
	p2 = Vector3::Add(Vector3::Multiply(1.0f - t2, seg2.origin_), Vector3::Multiply(t2, Vector3::Add(seg2.origin_, seg2.diff_)));
	len = PointSegmentDist(p2, seg1, p1, t1);
	if (0.0f <= t1 && t1 <= 1.0f) {
		return len;
	}

	// 双方の端点が最短と判明
	t1 = std::clamp(t1, 0.0f, 1.0f);
	p1 = Vector3::Add(Vector3::Multiply(1.0f - t1, seg1.origin_), Vector3::Multiply(t1, Vector3::Add(seg1.origin_, seg1.diff_)));
	return Vector3::Length(Vector3::Subtract(p2, p1));

}

