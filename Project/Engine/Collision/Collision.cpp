#include "Collision.h"
#include <cmath>
#include <algorithm>
#include <cassert>
#include "../Math/ShortestDistance.h"

bool Collision::IsCollision(const Sphere& sphere1, const Sphere& sphere2, Vector3& p1, Vector3& p2, float& t1, float& t2, float& pushBackDist)
{

	p1;
	p2;
	t1;
	t2;
	pushBackDist;

	//2つの球の中心点間の距離を求める
	float distance = Vector3::Length(Vector3::Subtract(sphere1.center_, sphere2.center_));
	//半径の合計よりも短ければ衝突
	if (distance <= sphere1.radius_ + sphere2.radius_) {
		return true;
	}

	return false;

}

bool Collision::IsCollision(const AABB& aabb1, const AABB& aabb2, Vector3& p1, Vector3& p2, float& t1, float& t2, float& pushBackDist)
{

	p1;
	p2;
	t1;
	t2;
	pushBackDist;

	if ((aabb1.min_.x <= aabb2.max_.x && aabb1.max_.x >= aabb2.min_.x) &&
		(aabb1.min_.y <= aabb2.max_.y && aabb1.max_.y >= aabb2.min_.y) &&
		(aabb1.min_.z <= aabb2.max_.z && aabb1.max_.z >= aabb2.min_.z)) {
		return true;
	}

	return false;

}

bool Collision::IsCollision(const OBB& obb, const OBB& obb2, Vector3& p1, Vector3& p2, float& t1, float& t2, float& pushBackDist)
{

	p1;
	p2;
	t1;
	t2;
	pushBackDist;

	//軸
	Vector3 axis[15];

	//面法線
	axis[0] = obb.otientatuons_[0];
	axis[1] = obb.otientatuons_[1];
	axis[2] = obb.otientatuons_[2];
	axis[3] = obb2.otientatuons_[0];
	axis[4] = obb2.otientatuons_[1];
	axis[5] = obb2.otientatuons_[2];

	//クロス積
	axis[6] = Vector3::Cross(obb.otientatuons_[0], obb2.otientatuons_[0]);
	axis[7] = Vector3::Cross(obb.otientatuons_[0], obb2.otientatuons_[1]);
	axis[8] = Vector3::Cross(obb.otientatuons_[0], obb2.otientatuons_[2]);
	axis[9] = Vector3::Cross(obb.otientatuons_[1], obb2.otientatuons_[0]);
	axis[10] = Vector3::Cross(obb.otientatuons_[1], obb2.otientatuons_[1]);
	axis[11] = Vector3::Cross(obb.otientatuons_[1], obb2.otientatuons_[2]);
	axis[12] = Vector3::Cross(obb.otientatuons_[2], obb2.otientatuons_[0]);
	axis[13] = Vector3::Cross(obb.otientatuons_[2], obb2.otientatuons_[1]);
	axis[14] = Vector3::Cross(obb.otientatuons_[2], obb2.otientatuons_[2]);

	//頂点

	//回転させる
	Vector3 obbVertex[8];

	obbVertex[0] = {
		+obb.size_.x,
		+obb.size_.y,
		+obb.size_.z };

	obbVertex[1] = {
		+obb.size_.x,
		+obb.size_.y,
		-obb.size_.z };

	obbVertex[2] = {
		+obb.size_.x,
		-obb.size_.y,
		+obb.size_.z };

	obbVertex[3] = {
		+obb.size_.x,
		-obb.size_.y,
		-obb.size_.z };

	obbVertex[4] = {
		-obb.size_.x,
		+obb.size_.y,
		+obb.size_.z };

	obbVertex[5] = {
		-obb.size_.x,
		+obb.size_.y,
		-obb.size_.z };

	obbVertex[6] = {
		-obb.size_.x,
		-obb.size_.y,
		+obb.size_.z };

	obbVertex[7] = {
		-obb.size_.x,
		-obb.size_.y,
		-obb.size_.z };

	Matrix4x4 obbRotateMatrix = {
		obb.otientatuons_[0].x,obb.otientatuons_[0].y,obb.otientatuons_[0].z,0.0f,
		obb.otientatuons_[1].x,obb.otientatuons_[1].y,obb.otientatuons_[1].z,0.0f,
		obb.otientatuons_[2].x,obb.otientatuons_[2].y,obb.otientatuons_[2].z,0.0f,
		0.0f,0.0f,0.0f,1.0f };

	for (int i = 0; i < 8; i++) {

		obbVertex[i] = Matrix4x4::Transform(obbVertex[i], obbRotateMatrix);
		obbVertex[i] = Vector3::Add(obbVertex[i], obb.center_);

	}

	Vector3 obb2Vertex[8];

	obb2Vertex[0] = {
		+obb2.size_.x,
		+obb2.size_.y,
		+obb2.size_.z };

	obb2Vertex[1] = {
		+obb2.size_.x,
		+obb2.size_.y,
		-obb2.size_.z };

	obb2Vertex[2] = {
		+obb2.size_.x,
		-obb2.size_.y,
		+obb2.size_.z };

	obb2Vertex[3] = {
		+obb2.size_.x,
		-obb2.size_.y,
		-obb2.size_.z };

	obb2Vertex[4] = {
		 -obb2.size_.x,
		+obb2.size_.y,
		+obb2.size_.z };

	obb2Vertex[5] = {
		-obb2.size_.x,
		+obb2.size_.y,
		-obb2.size_.z };

	obb2Vertex[6] = {
		-obb2.size_.x,
		-obb2.size_.y,
		+obb2.size_.z };

	obb2Vertex[7] = {
		-obb2.size_.x,
		-obb2.size_.y,
		-obb2.size_.z };

	Matrix4x4 obb2RotateMatrix = {
	obb2.otientatuons_[0].x,obb2.otientatuons_[0].y,obb2.otientatuons_[0].z,0.0f,
	obb2.otientatuons_[1].x,obb2.otientatuons_[1].y,obb2.otientatuons_[1].z,0.0f,
	obb2.otientatuons_[2].x,obb2.otientatuons_[2].y,obb2.otientatuons_[2].z,0.0f,
	0.0f,0.0f,0.0f,1.0f };

	for (int i = 0; i < 8; i++) {

		obb2Vertex[i] = Matrix4x4::Transform(obb2Vertex[i], obb2RotateMatrix);
		obb2Vertex[i] = Vector3::Add(obb2Vertex[i], obb2.center_);

	}

	//1.頂点を軸に対して射影
	for (int a = 0; a < 15; a++) {
		float min1 = 0.0f;
		float max1 = 0.0f;
		float min2 = 0.0f;
		float max2 = 0.0f;

		for (int v = 0; v < 8; v++) {
			//一時保存
			float tmp = 0.0f;
			//obb
			tmp = Vector3::Dot(Vector3::Normalize(axis[a]), obbVertex[v]);
			//2.射影した点の最大値と最小値を求める
			if (v == 0 || min1 > tmp) {
				min1 = tmp;
			}
			if (v == 0 || max1 < tmp) {
				max1 = tmp;
			}

			//obb2
			tmp = Vector3::Dot(Vector3::Normalize(axis[a]), obb2Vertex[v]);
			//2.射影した点の最大値と最小値を求める
			if (v == 0 || min2 > tmp) {
				min2 = tmp;
			}
			if (v == 0 || max2 < tmp) {
				max2 = tmp;
			}
		}

		//3.差分の形状を分離軸に射影した長さ
		float L1 = max1 - min1;
		float L2 = max2 - min2;

		float sumSpan = L1 + L2;
		float LonSpan = (std::max)(max1, max2) - (std::min)(min1, min2);
		if (sumSpan < LonSpan) {
			//分離しているので分離軸
			return false;
		}

	}

	return true;

}

bool Collision::IsCollision(const Capsule& capsule1, const Capsule& capsule2, Vector3& p1, Vector3& p2, float& t1, float& t2, float& pushBackDist)
{

	float dist = ShortestDistance::SegmentSegmentDist(capsule1.segment_, capsule2.segment_, p1, p2, t1, t2);

	pushBackDist = capsule1.radius_ + capsule2.radius_ - dist;

	return (dist <= capsule1.radius_ + capsule2.radius_);

}

bool Collision::IsCollision(const AABB& aabb, const Sphere& sphere, Vector3& p1, Vector3& p2, float& t1, float& t2, float& pushBackDist)
{

	p1;
	p2;
	t1;
	t2;
	pushBackDist;

	//最近接点を求める
	Vector3 closestPoint{ std::clamp(sphere.center_.x, aabb.min_.x, aabb.max_.x),
		std::clamp(sphere.center_.y, aabb.min_.y, aabb.max_.y) ,
		std::clamp(sphere.center_.z, aabb.min_.z, aabb.max_.z) };

	//最近接点と球の中心との距離を求める
	float distance = Vector3::Length(Vector3::Subtract(closestPoint, sphere.center_));
	//距離が半径よりも小さければ衝突
	if (distance <= sphere.radius_) {
		return true;
	}

	return false;

}

bool Collision::IsCollision(const Sphere& sphere, const AABB& aabb, Vector3& p1, Vector3& p2, float& t1, float& t2, float& pushBackDist)
{
	return IsCollision(aabb, sphere, p1, p2, t1, t2, pushBackDist);
}

bool Collision::IsCollision(const OBB& obb, const Sphere& sphere, Vector3& p1, Vector3& p2, float& t1, float& t2, float& pushBackDist)
{

	Matrix4x4 obbWorldMatrix = {
	obb.otientatuons_[0].x,obb.otientatuons_[1].x, obb.otientatuons_[2].x, 0,
	obb.otientatuons_[0].y,obb.otientatuons_[1].y, obb.otientatuons_[2].y, 0,
	obb.otientatuons_[0].z,obb.otientatuons_[1].z, obb.otientatuons_[2].z, 0,
	obb.center_.x, obb.center_.y, obb.center_.z, 1 };

	Matrix4x4 obbWorldMatrixInverse = Matrix4x4::Inverse(obbWorldMatrix);

	Vector3 centerInOBBLocalSpace = Matrix4x4::Transform(sphere.center_, obbWorldMatrixInverse);

	AABB aabbOBBLocal;
	aabbOBBLocal.Initialize(Vector3{ -obb.size_.x,-obb.size_.y, -obb.size_.z }, obb.size_, static_cast<Null*>(nullptr));

	Sphere sphereOBBLocal;
	sphereOBBLocal.Initialize(centerInOBBLocalSpace, sphere.radius_, static_cast<Null*>(nullptr));

	//ローカル空間で衝突
	return IsCollision(aabbOBBLocal, sphereOBBLocal, p1, p2, t1, t2, pushBackDist);

}

bool Collision::IsCollision(const Sphere& sphere, const OBB& obb, Vector3& p1, Vector3& p2, float& t1, float& t2, float& pushBackDist)
{
	return IsCollision(obb, sphere, p1, p2, t1, t2, pushBackDist);
}

bool Collision::IsCollision(const AABB& aabb, const OBB& obb, Vector3& p1, Vector3& p2, float& t1, float& t2, float& pushBackDist)
{
	aabb;
	obb;
	p1;
	p2;
	t1;
	t2;
	pushBackDist;

	assert(0);
	return false;
}

bool Collision::IsCollision(const OBB& obb, const AABB& aabb, Vector3& p1, Vector3& p2, float& t1, float& t2, float& pushBackDist)
{
	return IsCollision(aabb, obb, p1, p2, t1, t2, pushBackDist);
}

bool Collision::IsCollision(const Capsule& capsule, const Sphere& sphere, Vector3& p1, Vector3& p2, float& t1, float& t2, float& pushBackDist)
{
	capsule;
	sphere;
	p1;
	p2;
	t1;
	t2;
	pushBackDist;

	assert(0);
	return false;
}

bool Collision::IsCollision(const Sphere& sphere, const Capsule& capsule, Vector3& p1, Vector3& p2, float& t1, float& t2, float& pushBackDist)
{
	return IsCollision(capsule, sphere, p1, p2, t1, t2, pushBackDist);
}

bool Collision::IsCollision(const Capsule& capsule, const AABB& aabb, Vector3& p1, Vector3& p2, float& t1, float& t2, float& pushBackDist)
{

	// 計算用
	Vector3 start = capsule.segment_.origin_;
	Vector3 end = Vector3::Add(capsule.segment_.origin_, capsule.segment_.diff_);

	// 線分確認
	Vector3 tMin = {
		(aabb.min_.x - capsule.segment_.origin_.x) / capsule.segment_.diff_.x,
		(aabb.min_.y - capsule.segment_.origin_.y) / capsule.segment_.diff_.y,
		(aabb.min_.z - capsule.segment_.origin_.z) / capsule.segment_.diff_.z };

	Vector3 tMax = {
		(aabb.max_.x - capsule.segment_.origin_.x) / capsule.segment_.diff_.x,
		(aabb.max_.y - capsule.segment_.origin_.y) / capsule.segment_.diff_.y,
		(aabb.max_.z - capsule.segment_.origin_.z) / capsule.segment_.diff_.z };

	Vector3 tNear = { (std::min)(tMin.x, tMax.x) ,
		(std::min)(tMin.y, tMax.y) ,
		(std::min)(tMin.z, tMax.z) };

	Vector3 tFar = { (std::max)(tMin.x, tMax.x) ,
		(std::max)(tMin.y, tMax.y) ,
		(std::max)(tMin.z, tMax.z) };

	float tMin_ = (std::max)((std::max)(tNear.x, tNear.y), tNear.z);
	float tMax_ = (std::min)((std::min)(tFar.x, tFar.y), tFar.z);

	if (tMin_ <= tMax_) {

		if (tMin_ < 1.0f && tMax_ > 0.0f) {
			t1 = tMin_;
			p1 = Vector3::Add(Vector3::Multiply(1.0f - tMin_, start), Vector3::Multiply(tMin_, end));
			pushBackDist = capsule.radius_;
			return true;
		}
	}

	// クランプ
	tMin_ = std::clamp(tMin_, 0.0f, 1.0f);
	tMax_ = std::clamp(tMax_, 0.0f, 1.0f);

	Sphere sphere;
	sphere.Initialize({ 0.0f,0.0f,0.0f }, 0.0f, static_cast<Null*>(nullptr));

	sphere.center_ = Vector3::Add(Vector3::Multiply(1.0f - tMin_, start), Vector3::Multiply(tMin_, end));
	sphere.radius_ = capsule.radius_ ;
	if (IsCollision(aabb, sphere, p1, p2, t1, t2, pushBackDist)) {
		t1 = tMin_;
		p1 = sphere.center_;
		return true;
	}

	sphere.center_ = Vector3::Add(Vector3::Multiply(1.0f - tMax_, start), Vector3::Multiply(tMax_, end));
	sphere.radius_ = capsule.radius_;
	if (IsCollision(aabb, sphere, p1, p2, t1, t2, pushBackDist)) {
		t1 = tMax_;
		p1 = sphere.center_;
		return true;
	}

	return false;

}

bool Collision::IsCollision(const AABB& aabb, const Capsule& capsule, Vector3& p1, Vector3& p2, float& t1, float& t2, float& pushBackDist)
{
	return IsCollision(capsule, aabb, p1, p2, t1, t2, pushBackDist);
}

bool Collision::IsCollision(const Capsule& capsule, const OBB& obb, Vector3& p1, Vector3& p2, float& t1, float& t2, float& pushBackDist)
{

	Matrix4x4 obbWorldMatrix = {
	obb.otientatuons_[0].x,obb.otientatuons_[1].x, obb.otientatuons_[2].x, 0,
	obb.otientatuons_[0].y,obb.otientatuons_[1].y, obb.otientatuons_[2].y, 0,
	obb.otientatuons_[0].z,obb.otientatuons_[1].z, obb.otientatuons_[2].z, 0,
	obb.center_.x, obb.center_.y, obb.center_.z, 1 };

	Matrix4x4 obbWorldMatrixInverse = Matrix4x4::Inverse(obbWorldMatrix);

	Vector3 localorigin_ = Matrix4x4::Transform(capsule.segment_.origin_, obbWorldMatrixInverse);
	Vector3 localEnd = Matrix4x4::Transform(Vector3::Add(capsule.segment_.origin_, capsule.segment_.diff_), obbWorldMatrixInverse);

	Capsule localCapsule = {};
	localCapsule.radius_ = capsule.radius_;
	localCapsule.segment_.origin_ = localorigin_;
	localCapsule.segment_.diff_ = Vector3::Subtract(localEnd, localorigin_);

	AABB localAABB;
	localAABB.Initialize(
		{ -obb.size_.x, -obb.size_.y, -obb.size_.z },
		{ obb.size_.x, obb.size_.y, obb.size_.z }, 
		static_cast<Null*>(nullptr));

	bool result = IsCollision(localCapsule, localAABB, p1, p2, t1, t2, pushBackDist);

	p1 = Matrix4x4::Transform(p1, obbWorldMatrix);

	return result;

}

bool Collision::IsCollision(const OBB& obb, const Capsule& capsule, Vector3& p1, Vector3& p2, float& t1, float& t2, float& pushBackDist)
{
	return IsCollision(capsule, obb, p1, p2, t1, t2, pushBackDist);
}
