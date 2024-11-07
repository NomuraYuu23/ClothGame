#include "FollowCamera.h"
#include "../../../Engine/Math/Vector/Vector3.h"
#include "../../../Engine/Math/Matrix/Matrix4x4.h"
#include "../../../Engine/Input/input.h"
#include <algorithm>
#include "../../../Engine/GlobalVariables/GlobalVariables.h"
#include "../../../Engine/Math/Math.h"
#include "../../../Engine/Math/Ease.h"
#include "../../../Engine/Input/Input.h"

void FollowCamera::Initialize() {

	BaseCamera::Initialize();

	BaseCamera::Update();

	// 追従対象の残像座標
	interTarget_ = { 0.0f,0.0f,0.0f };

	// 目指すアングル
	destinationAngle_ = { 0.0f,0.0f,0.0f };

	// 移動レート
	moveRate_ = 0.2f;

	// 回転レート
	rotateRate_ = 0.1f;

	// オフセットの長さ
	offsetLength_ = -10.0f;

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "FollowCamera";
	//グループを追加
	GlobalVariables::GetInstance()->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "moveRate", moveRate_);
	globalVariables->AddItem(groupName, "rotateRate", rotateRate_);
	globalVariables->AddItem(groupName, "offsetLength", offsetLength_);

	ApplyGlobalVariables();

}

void FollowCamera::Update(float elapsedTime) {

#ifdef _DEBUG
	ApplyGlobalVariables();
#endif // _DEBUG


	NotLockOnUpdate();


	//追従対象がいれば
	if (target_) {
		// 追従座標の補間
		Vector3 targetPos = { target_->worldMatrix_.m[3][0], target_->worldMatrix_.m[3][1], target_->worldMatrix_.m[3][2] };
		interTarget_ = Ease::Easing(Ease::EaseName::Lerp, interTarget_, targetPos, moveRate_);

		// オフセット
		Vector3 offset = OffsetCalc();

		transform_.translate = Vector3::Add(interTarget_, offset);

	}

	// ズーム
	Zoom(elapsedTime);

	// シェイク
	if (isShake_) {
		ShakeUpdate(elapsedTime);
	}

	// マッピング
	Matrix4x4 scaleMatrix = Matrix4x4::MakeScaleMatrix(transform_.scale);
	Matrix4x4 rotateMatrix = GetRotateMatrix();
	Matrix4x4 translateMatrix = Matrix4x4::MakeTranslateMatrix(transform_.translate + shakeAddPosition_);

	transformMatrix_ = Matrix4x4::Multiply(scaleMatrix, Matrix4x4::Multiply(rotateMatrix, translateMatrix));
	viewMatrix_ = Matrix4x4::Inverse(transformMatrix_);
	projectionMatrix_ = Matrix4x4::MakePerspectiveFovMatrix(fovY_, aspectRatio_, nearClip_, farClip_);

	viewProjectionMatrix_->matrix = Matrix4x4::Multiply(viewMatrix_, projectionMatrix_);

	worldPositionMap_->worldPosition = { transformMatrix_.m[3][0],transformMatrix_.m[3][1], transformMatrix_.m[3][2] };


}

void FollowCamera::SetTarget(const WorldTransform* target)
{

	target_ = target;

	// 追従座標の補間
	Vector3 targetPos = { target_->worldMatrix_.m[3][0], target_->worldMatrix_.m[3][1], target_->worldMatrix_.m[3][2] };
	interTarget_ = targetPos;

	// オフセット
	Vector3 offset = OffsetCalc();

	transform_.translate = Vector3::Add(interTarget_, offset);

	//ビュー更新
	BaseCamera::Update();

}

Matrix4x4 FollowCamera::GetRotateMatrix()
{

	return Matrix4x4::MakeRotateXYZMatrix(transform_.rotate);

}


Vector3 FollowCamera::OffsetCalc()
{

	//追従対象からカメラまでのオフセット
	Vector3 offset = { 0.0f, 3.0f, offsetLength_ };

	Matrix4x4 rotateMatrix = GetRotateMatrix();

	//オフセットをカメラの回転に合わせて回転させる
	offset = Matrix4x4::TransformNormal(offset, rotateMatrix);

	return offset;

}

void FollowCamera::NotLockOnUpdate()
{

	//インスタンス
	Input* input = Input::GetInstance();

	// スティック入力で角度を変更処理
	// 回転速度
	const float RotateSpeed = 0.000003f;

	destinationAngle_.y += input->GetRightAnalogstick().x * RotateSpeed;
	destinationAngle_.x += input->GetRightAnalogstick().y * RotateSpeed;

	// xに制限
	float limit = 3.14f / 4.0f;
	destinationAngle_.x = std::clamp(destinationAngle_.x, 0.0f, limit);

	// 回転を決定
	transform_.rotate.y = Math::LerpShortAngle(transform_.rotate.y, destinationAngle_.y, rotateRate_);
	transform_.rotate.x = Math::LerpShortAngle(transform_.rotate.x, destinationAngle_.x, rotateRate_);

}

void FollowCamera::ApplyGlobalVariables()
{

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "FollowCamera";

	moveRate_ = globalVariables->GetFloatValue(groupName, "moveRate");
	rotateRate_ = globalVariables->GetFloatValue(groupName, "rotateRate");
	offsetLength_ = globalVariables->GetFloatValue(groupName, "offsetLength");

}
