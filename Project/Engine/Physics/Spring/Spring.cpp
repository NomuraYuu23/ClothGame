#include "Spring.h"
#include "../../Math/DeltaTime.h"

void Spring::Initialize(
	const Vector3& anchor, 
	float naturalLength, 
	float stiffness, 
	float dampingCoefficient,
	const MassPoint& massPoint)
{

	anchor_ = anchor;

	naturalLength_ = naturalLength;

	stiffness_ = stiffness;

	dampingCoefficient_ = dampingCoefficient;

	massPoint_ = massPoint;

}

void Spring::Update()
{

	Vector3 diff = massPoint_.position_ - anchor_;
	float length = Vector3::Length(diff);
	if (length != 0.0f) {
		Vector3 direction = Vector3::Normalize(diff);
		Vector3 restPosition = anchor_ + direction * naturalLength_;
		Vector3 displacement = Vector3::Multiply(length,(massPoint_.position_ - restPosition));
		Vector3 restoringForce = Vector3::Multiply(-stiffness_,displacement);
		Vector3 dampingForce = Vector3::Multiply(-dampingCoefficient_,massPoint_.velocity_);
		Vector3 force = restoringForce + dampingForce;
		massPoint_.acceleration_ = Vector3::Multiply(force, 1.0f / massPoint_.mass_)
			+ Vector3{ 0.0f, -9.8f, 0.0f } * massPoint_.mass_;
	}

	massPoint_.velocity_ = massPoint_.velocity_ + massPoint_.acceleration_ * kDeltaTime_;
	massPoint_.position_ = massPoint_.position_ + massPoint_.velocity_ * kDeltaTime_;

}
