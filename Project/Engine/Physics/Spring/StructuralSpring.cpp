#include "StructuralSpring.h"
#include "../../Math/DeltaTime.h"
#include <algorithm>

void StructuralSpring::Initialize(
	const MassPoint& point0, 
	const MassPoint& point1, 
	float naturalLength, 
	float stiffness, 
	float dampingCoefficient)
{

	// 質点0
	point0_ = point0;
	// 質点1
	point1_ = point1;
	// 自然長
	naturalLength_ = naturalLength;
	// 剛性。バネ定数k
	stiffness_ = stiffness;
	// 減衰係数
	dampingCoefficient_ = dampingCoefficient;

	// 質点0を固定するか
	fixPoint0_ = false;
	// 質点1を固定するか
	fixPoint1_ = false;

	// 最大距離
	lengthMax_ = Vector3::Length(Vector3::Subtract(point0.position, point1.position));

}

void StructuralSpring::Update(
	const Vector3& wind,
	const Vector3& gravity)
{

	MassPoint point0 = point0_;
	MassPoint point1 = point1_;

	// point0_
	if (!fixPoint0_) {
		Vector3 diff = point0.position - point1.position;
		float length = Vector3::Length(diff);
		if (length != 0.0f) {
			Vector3 direction = Vector3::Normalize(diff);
			Vector3 restPosition = point1.position + direction * naturalLength_;
			Vector3 displacement = Vector3::Multiply(length, (point0.position - restPosition));
			Vector3 restoringForce = Vector3::Multiply(-stiffness_, displacement);
			Vector3 dampingForce = Vector3::Multiply(-dampingCoefficient_, point0.velocity);
			Vector3 force = restoringForce + dampingForce;

			point0_.acceleration = Vector3::Multiply(force, 1.0f / point0_.mass)
				+ (Vector3::Add(gravity, wind)) * point0_.mass;
		}
		point0_.velocity = point0_.velocity + point0_.acceleration * kDeltaTime_;
	}
	else {
		point0_.velocity = { 0.0f,0.0f,0.0f };
	}

	// point1_
	if (!fixPoint1_) {
		Vector3 diff = point1.position - point0.position;
		float length = Vector3::Length(diff);
		if (length != 0.0f) {
			Vector3 direction = Vector3::Normalize(diff);
			Vector3 restPosition = point0.position + direction * naturalLength_;
			Vector3 displacement = Vector3::Multiply(length, (point1.position - restPosition));
			Vector3 restoringForce = Vector3::Multiply(-stiffness_, displacement);
			Vector3 dampingForce = Vector3::Multiply(-dampingCoefficient_, point1.velocity);
			Vector3 force = restoringForce + dampingForce;

			point1_.acceleration = Vector3::Multiply(force, 1.0f / point1_.mass)
				+ (Vector3::Add(gravity, wind)) * point1_.mass;
		}
		point1_.velocity = point1_.velocity + point1_.acceleration * kDeltaTime_;
	}
	else {
		point1_.velocity = { 0.0f,0.0f,0.0f };
	}

	// 速度制限
	const float velocityRestrictions = 64.0f;

	point0_.velocity.x = std::clamp(point0_.velocity.x, -velocityRestrictions, velocityRestrictions);
	point0_.velocity.y = std::clamp(point0_.velocity.y, -velocityRestrictions, velocityRestrictions);
	point0_.velocity.z = std::clamp(point0_.velocity.z, -velocityRestrictions, velocityRestrictions);

	point1_.velocity.x = std::clamp(point1_.velocity.x, -velocityRestrictions, velocityRestrictions);
	point1_.velocity.y = std::clamp(point1_.velocity.y, -velocityRestrictions, velocityRestrictions);
	point1_.velocity.z = std::clamp(point1_.velocity.z, -velocityRestrictions, velocityRestrictions);

	// 位置変動
	point0_.position = point0_.position + point0_.velocity * kDeltaTime_;
	point1_.position = point1_.position + point1_.velocity * kDeltaTime_;

}

void StructuralSpring::PositionLimit()
{

	float length = Vector3::Length(Vector3::Subtract(point1_.position, point0_.position));

	if (length <= lengthMax_) {
		return;
	}

	float ratio = lengthMax_ / length;

	length = ratio * length;

	Vector3 addPos = Vector3::Normalize(Vector3::Subtract(point1_.position, point0_.position)) * length;

	point1_.position = point0_.position + addPos;

}

void StructuralSpring::SetName(const std::string& name)
{

	name_ = name;

	std::string y = "";
	std::string x = "";
	size_t tmpIndex = 0;

	// 名前取得
	std::string nameTmp = name_;

	// 名前分割

	// "_"の位置を取得
	tmpIndex = nameTmp.find_first_of("_", 0);
	// "_"のひとつ前までを代入
	kinds_ = nameTmp.substr(0, tmpIndex);
	// 名前を"_"の一つ先からに変更する
	nameTmp = nameTmp.substr(tmpIndex + 1);

	// "_"の位置を取得
	tmpIndex = nameTmp.find_first_of("_", 0);
	// "_"のひとつ前までを代入
	axis_ = nameTmp.substr(0, tmpIndex );
	// 名前を"_"の一つ先からに変更する
	nameTmp = nameTmp.substr(tmpIndex + 1);

	// "_"の位置を取得
	tmpIndex = nameTmp.find_first_of("_", 0);
	// "_"のひとつ前までを代入
	y = nameTmp.substr(0, tmpIndex);
	// 名前を"_"の一つ先からに変更する
	nameTmp = nameTmp.substr(tmpIndex + 1);

	// "_"の位置を取得
	tmpIndex = nameTmp.find_first_of("_", 0);
	// "_"のひとつ前までを代入
	x = nameTmp.substr(0, tmpIndex);
	// 名前を"_"の一つ先からに変更する
	nameTmp = nameTmp.substr(tmpIndex + 1);


	y_ = atoi(y.c_str());
	x_ = atoi(x.c_str());

}
