#include "BaseCamera.h"
#include "../base/WinApp.h"
#include "../base/BufferResource.h"
#include "../base/DxCommon/DirectXCommon.h"
#include "../Math/RandomEngine.h"
#include "../Math/Ease.h"
#include "../Math/DeltaTime.h"

void BaseCamera::Initialize()
{

	transform_ = { { 1.0f, 1.0f, 1.0f},{ 0.0f, 0.0f, 0.0f},{ 0.0f, 0.0f, -10.0f} };

	fovY_ = 0.45f;
	targetFovY_ = 0.45f;
	zoomT_ = 0.05f;

	aspectRatio_ = float(WinApp::kWindowWidth) / float(WinApp::kWindowHeight);

	nearClip_ = 0.1f;

	farClip_ = 1000.0f;

	//WVP用のリソースを作る。Matrix4x4 1つ分のサイズを用意する
	worldPositionBuff_ = BufferResource::CreateBufferResource(DirectXCommon::GetInstance()->GetDevice(), (sizeof(CameraForGPU) + 0xff) & ~0xff);
	//書き込むためのアドレスを取得
	worldPositionBuff_->Map(0, nullptr, reinterpret_cast<void**>(&worldPositionMap_));

	//WVP用のリソースを作る。Matrix4x4 1つ分のサイズを用意する
	viewProjectionMatrixBuff_ = BufferResource::CreateBufferResource(DirectXCommon::GetInstance()->GetDevice(), (sizeof(ViewProjectionMatrix) + 0xff) & ~0xff);
	viewProjectionMatrixBuff_->Map(0, nullptr, reinterpret_cast<void**>(&viewProjectionMatrix_));

	// シェイクしているか
	isShake_ = false;
	// シェイクの大きさ(絶対値)
	shakeSize_ = 0.0f;
	// シェイク初期時間
	shakeInitTime_ = 0.0f;
	// シェイク時間
	shakeTime_ = 0.0f;
	// シェイク追加位置
	shakeAddPosition_ = { 0.0f,0.0f,0.0f };

	BaseCamera::Update();

}

void BaseCamera::Update(float elapsedTime)
{

	// ズーム
	Zoom(elapsedTime);

	// シェイク
	if (isShake_) {
		ShakeUpdate(elapsedTime);
	}

	// マッピング
	transformMatrix_ = Matrix4x4::MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate + shakeAddPosition_);
	viewMatrix_ = Matrix4x4::Inverse(transformMatrix_);
	projectionMatrix_ = Matrix4x4::MakePerspectiveFovMatrix(fovY_, aspectRatio_, nearClip_, farClip_);

	viewProjectionMatrix_->matrix = Matrix4x4::Multiply(viewMatrix_, projectionMatrix_);

	worldPositionMap_->worldPosition = { transformMatrix_.m[3][0],transformMatrix_.m[3][1], transformMatrix_.m[3][2] };

}

void BaseCamera::ShakeStart(float shakeSize, float shakeTime)
{

	shakeSize_ = shakeSize;
	shakeInitTime_ = shakeTime;
	shakeTime_ = shakeTime;
	isShake_ = true;

}

void BaseCamera::ShakeStop()
{

	shakeSize_ = 0.0f;
	shakeInitTime_ = 0.0f;
	shakeTime_ = 0.0f;
	shakeAddPosition_ = { 0.0f,0.0f,0.0f };
	isShake_ = false;

}

void BaseCamera::ShakeUpdate(float elapsedTime)
{

	// シェイクの大きさ
	const float shakeSizeMax =
		shakeSize_ * (shakeTime_ / shakeInitTime_);

	// シェイク
	shakeAddPosition_.x =
		RandomEngine::GetRandom(-shakeSizeMax, shakeSizeMax);
	shakeAddPosition_.y =
		RandomEngine::GetRandom(-shakeSizeMax, shakeSizeMax);
	shakeAddPosition_.z =
		RandomEngine::GetRandom(-shakeSizeMax, shakeSizeMax);

	// 時間処理
	shakeTime_ -= elapsedTime;
	if (shakeTime_ <= 0.0f) {
		ShakeStop();
	}

}

void BaseCamera::Zoom(float elapsedTime)
{
	
	fovY_ = Ease::Easing(Ease::EaseName::Lerp, fovY_, targetFovY_, zoomT_ * (elapsedTime / kDeltaTime_));

}
