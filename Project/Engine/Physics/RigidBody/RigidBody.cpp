#include "RigidBody.h"
#include "InertiaTensor.h"
#include "../Gravity/Gravity.h"

void RigidBody::Initialize(float mass, const Vector3& size)
{

	// 慣性テンソル作成
	inertiaTensor = InertiaTensor::CreateRectangular(mass, size);

	// 基本姿勢での慣性テンソル作成
	basicPostureInertiaTensor = inertiaTensor;

	// 姿勢行列作成
	postureMatrix = Matrix4x4::MakeRotateXYZMatrix({ 0.0f, 0.0f, 0.0f });

	angularVelocity = { 0.0f,0.0f,0.0f }; // 角速度
	angularMomentum = { 0.0f,0.0f,0.0f }; // 角運動量

	centerOfGravityVelocity = { 0.0f,0.0f,0.0f }; // 重心位置速度

}

void RigidBody::ApplyForce(const Vector3& center, const Vector3& pointOfAction, const Vector3& force)
{

	centerOfGravity = center;
	torque = RigidBody::TorqueCalc(centerOfGravity, pointOfAction, force);

}

void RigidBody::CollisionPositionConfirmation(
	RigidBody* rigidBody, 
	const OBB& myObb, 
	const OBB& pairObb, 
	float coefficientOfRestitution,
	bool isGround,
	float power)
{

	// 地面に近い点を2点求める
	Vector3 obbVertex[8];

	obbVertex[0] = {
		+myObb.size_.x,
		+myObb.size_.y,
		+myObb.size_.z };

	obbVertex[1] = {
		+myObb.size_.x,
		+myObb.size_.y,
		-myObb.size_.z };

	obbVertex[2] = {
		+myObb.size_.x,
		-myObb.size_.y,
		+myObb.size_.z };

	obbVertex[3] = {
		+myObb.size_.x,
		-myObb.size_.y,
		-myObb.size_.z };

	obbVertex[4] = {
		-myObb.size_.x,
		+myObb.size_.y,
		+myObb.size_.z };

	obbVertex[5] = {
		-myObb.size_.x,
		+myObb.size_.y,
		-myObb.size_.z };

	obbVertex[6] = {
		-myObb.size_.x,
		-myObb.size_.y,
		+myObb.size_.z };

	obbVertex[7] = {
		-myObb.size_.x,
		-myObb.size_.y,
		-myObb.size_.z };

	Matrix4x4 obbRotateMatrix = {
		myObb.otientatuons_[0].x,myObb.otientatuons_[0].y,myObb.otientatuons_[0].z,0.0f,
		myObb.otientatuons_[1].x,myObb.otientatuons_[1].y,myObb.otientatuons_[1].z,0.0f,
		myObb.otientatuons_[2].x,myObb.otientatuons_[2].y,myObb.otientatuons_[2].z,0.0f,
		0.0f,0.0f,0.0f,1.0f };

	for (uint32_t i = 0; i < 8; i++) {

		obbVertex[i] = Matrix4x4::Transform(obbVertex[i], obbRotateMatrix);
		obbVertex[i] = Vector3::Add(obbVertex[i], myObb.center_);

	}

	// ペアに近い点、遠い点をそれぞれ二つ見つける
	// 力の設定
	uint32_t closeNumber = 0;
	uint32_t closeNumber2 = 0;

	uint32_t farNumber = 0;
	uint32_t farNumber2 = 0;

	Vector3 force = { 0.0f,0.0f,0.0f };
	if (isGround) {
		FindTwoClosePoints(closeNumber, closeNumber2, obbVertex);
		FindTwoFarPoints(farNumber, farNumber2, obbVertex);
		force.y = -power;
	}
	else {
		rigidBody->centerOfGravityVelocity.y = 0.0f; // 重力があるため0にしないと変になる
		return;
	}

	rigidBody->ApplyForce(
		(obbVertex[closeNumber] + obbVertex[closeNumber2]) * 0.5f, 
		(obbVertex[farNumber] + obbVertex[farNumber2]) * 0.5f,
		force);

	// 反発
	rigidBody->centerOfGravityVelocity = rigidBody->centerOfGravityVelocity * -coefficientOfRestitution;
	
	// 小さい値だったら動かさない
	if (std::fabsf(rigidBody->centerOfGravityVelocity.x) < Gravity::GetPower() * kDeltaTime_) {
		rigidBody->centerOfGravityVelocity.x = 0.0f;
	}
	if (std::fabsf(rigidBody->centerOfGravityVelocity.y) < Gravity::GetPower() * kDeltaTime_) {
		rigidBody->centerOfGravityVelocity.y = 0.0f;
	}
	if (std::fabsf(rigidBody->centerOfGravityVelocity.z) < Gravity::GetPower() * kDeltaTime_) {
		rigidBody->centerOfGravityVelocity.z = 0.0f;
	}

}

void RigidBody::FindTwoClosePoints(uint32_t& ansNumber, uint32_t& ansNumber2, const Vector3 obbVertex[8], const Vector3& pairPos)
{

	float minDistance = 0.0f;
	float minDistance2 = 0.0f;

	float distanceTmp = 0.0f;

	minDistance = Vector3::Length(Vector3::Subtract(obbVertex[0], pairPos));
	ansNumber = 0;
	minDistance2 = Vector3::Length(Vector3::Subtract(obbVertex[1], pairPos));
	ansNumber2 = 1;

	if (minDistance > minDistance2) {
		distanceTmp = minDistance;

		minDistance = minDistance2;
		ansNumber = 1;
		minDistance2 = distanceTmp;
		ansNumber2 = 0;
	}

	for (uint32_t i = 2; i < 8; i++) {

		distanceTmp = Vector3::Length(Vector3::Subtract(obbVertex[i], pairPos));

		if (minDistance > distanceTmp) {
			minDistance2 = minDistance;
			ansNumber2 = ansNumber;
			minDistance = distanceTmp;
			ansNumber = i;
		}
		else if (minDistance2 > distanceTmp) {
			minDistance2 = distanceTmp;
			ansNumber2 = i;
		}

	}

}

void RigidBody::FindTwoClosePoints(uint32_t& ansNumber, uint32_t& ansNumber2, const Vector3 obbVertex[8])
{

	// 保存用
	float minY = 0.0f;
	float minY2 = 0.0f;

	if (obbVertex[0].y > obbVertex[1].y) {
		minY = obbVertex[1].y;
		ansNumber = 1;
		minY2 = obbVertex[0].y;
		ansNumber2 = 0;
	}
	else {
		minY = obbVertex[0].y;
		ansNumber = 0;
		minY2 = obbVertex[1].y;
		ansNumber2 = 1;
	}

	for (uint32_t i = 2; i < 8; i++) {

		if (minY > obbVertex[i].y) {
			minY2 = minY;
			ansNumber2 = ansNumber;
			minY = obbVertex[i].y;
			ansNumber = i;
		}
		else if (minY2 > obbVertex[i].y) {
			minY2 = obbVertex[i].y;
			ansNumber2 = i;
		}

	}

}

void RigidBody::FindTwoFarPoints(uint32_t& ansNumber, uint32_t& ansNumber2, const Vector3 obbVertex[8], const Vector3& pairPos)
{


	float maxDistance = 0.0f;
	float maxDistance2 = 0.0f;

	float distanceTmp = 0.0f;

	maxDistance = Vector3::Length(Vector3::Subtract(obbVertex[0], pairPos));
	ansNumber = 0;
	maxDistance2 = Vector3::Length(Vector3::Subtract(obbVertex[1], pairPos));
	ansNumber2 = 1;

	if (maxDistance < maxDistance2) {
		distanceTmp = maxDistance;

		maxDistance = maxDistance2;
		ansNumber = 1;
		maxDistance2 = distanceTmp;
		ansNumber2 = 0;
	}

	for (uint32_t i = 2; i < 8; i++) {

		distanceTmp = Vector3::Length(Vector3::Subtract(obbVertex[i], pairPos));

		if (maxDistance < distanceTmp) {
			maxDistance2 = maxDistance;
			ansNumber2 = ansNumber;
			maxDistance = distanceTmp;
			ansNumber = i;
		}
		else if (maxDistance2 < distanceTmp) {
			maxDistance2 = distanceTmp;
			ansNumber2 = i;
		}

	}

}

void RigidBody::FindTwoFarPoints(uint32_t& ansNumber, uint32_t& ansNumber2, const Vector3 obbVertex[8])
{

	// 保存用
	float maxY = 0.0f;
	float maxY2 = 0.0f;

	if (obbVertex[0].y < obbVertex[1].y) {
		maxY = obbVertex[1].y;
		ansNumber = 1;
		maxY2 = obbVertex[0].y;
		ansNumber2 = 0;
	}
	else {
		maxY = obbVertex[0].y;
		ansNumber = 0;
		maxY2 = obbVertex[1].y;
		ansNumber2 = 1;
	}

	for (uint32_t i = 2; i < 8; i++) {

		if (maxY < obbVertex[i].y) {
			maxY2 = maxY;
			ansNumber2 = ansNumber;
			maxY = obbVertex[i].y;
			ansNumber = i;
		}
		else if (maxY2 < obbVertex[i].y) {
			maxY2 = obbVertex[i].y;
			ansNumber2 = i;
		}

	}

}

Vector3 RigidBody::TorqueCalc(
	const Vector3& centerOfGravity,
	const Vector3& pointOfAction,
	const Vector3& force)
{

	Vector3 result;
	Vector3 r = Vector3::Subtract(pointOfAction, centerOfGravity);
	result = Vector3::Cross(r, force);
	return result;

}

Matrix4x4 RigidBody::InertiaTensorCalc(
	const Matrix4x4& postureMatrix,
	const Matrix4x4& basicPostureInertiaTensor)
{

	Matrix4x4 result;

	Matrix4x4 transposePostureMatrix = Matrix4x4::Transpose(postureMatrix);

	result = Matrix4x4::Multiply(Matrix4x4::Multiply(postureMatrix, basicPostureInertiaTensor), transposePostureMatrix);

	return result;
}

Matrix4x4 RigidBody::PostureCalc(
	const Matrix4x4& postureMatrix,
	const Vector3& angularVelocity,
	float time)
{

	Matrix4x4 newPostureMatrix = Matrix4x4::MakeRotateXYZMatrix(Vector3::Multiply(angularVelocity, time));

	Matrix4x4 result = Matrix4x4::Multiply(newPostureMatrix, postureMatrix);
	result.m[3][3] = 1.0f;

	return result;

}

Vector3 RigidBody::AngularMomentumCalc(
	const Vector3& angularMomentum,
	const Vector3& torque,
	float time)
{

	Vector3 result;
	result = Vector3::Add(angularMomentum, Vector3::Multiply(torque, time));
	return result;

}

Vector3 RigidBody::AngularVelocityCalc(
	const Matrix4x4& inertiaTensor,
	const Vector3& angularMomentum)
{

	Vector3 result;
	result = Matrix4x4::Transform(angularMomentum, Matrix4x4::Inverse(inertiaTensor));
	return result;

}

Vector3 RigidBody::PointVelocityCalc(
	const Vector3& angularVelocity,
	const Vector3& centerOfGravityVelocity,
	const Vector3& point,
	const Vector3& centerOfGravity)
{

	// 重心から剛体上の点pointに向かうベクトル
	Vector3 r = Vector3::Subtract(point, centerOfGravity);

	return Vector3::Add(centerOfGravityVelocity, Vector3::Cross(angularVelocity, r));

}

RigidBody::Jacobian RigidBody::JacobianCalc(
	const Vector3& normalize, 
	const Vector3& point, 
	const Vector3& centerOfGravity)
{

	// 重心から剛体上の点pointに向かうベクトル
	Vector3 r = Vector3::Subtract(point, centerOfGravity);
	
	Jacobian result{};

	result.n = normalize;
	result.nR = Vector3::Cross(r, normalize);

	return result;

}

bool RigidBody::RestraintConfirmationJoint(
	const Vector3& velocityA, 
	const Vector3& velocityB)
{

	// 速度が同じなら拘束出来ている
	if (velocityA.x == velocityB.x &&
		velocityA.y == velocityB.y &&
		velocityA.z == velocityB.z) {

		return true;

	}

	return false;

}

bool RigidBody::RestraintConfirmationNoPenetration(
	const Vector3& velocityA, 
	const Vector3& velocityB, 
	const Vector3& normalizeB)
{

	// 速度と法線で内積を出す
	float dot = Vector3::Dot(normalizeB, Vector3::Subtract(velocityA, velocityB));

	// 内積が0より大きいなら拘束出来ている
	if (dot >= 0.0f) {

		return true;

	}

	return false;

}
