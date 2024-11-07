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

	Vector3 diff = massPoint_.position - anchor_;
	float length = Vector3::Length(diff);
	if (length != 0.0f) {
		Vector3 direction = Vector3::Normalize(diff);
		Vector3 restPosition = anchor_ + direction * naturalLength_;
		Vector3 displacement = Vector3::Multiply(length,(massPoint_.position - restPosition));
		Vector3 restoringForce = Vector3::Multiply(-stiffness_,displacement);
		Vector3 dampingForce = Vector3::Multiply(-dampingCoefficient_,massPoint_.velocity);
		Vector3 force = restoringForce + dampingForce;
		massPoint_.acceleration = Vector3::Multiply(force, 1.0f / massPoint_.mass)
			+ Vector3{ 0.0f, -9.8f, 0.0f } * massPoint_.mass;
	}

	massPoint_.velocity = massPoint_.velocity + massPoint_.acceleration * kDeltaTime_;
	massPoint_.position = massPoint_.position + massPoint_.velocity * kDeltaTime_;

}
