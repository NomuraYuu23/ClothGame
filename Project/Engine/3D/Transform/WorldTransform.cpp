#include "WorldTransform.h"

#include "../../base/BufferResource.h"
#include "../../base/WinApp.h"
#include "../../base/DxCommon/DirectXCommon.h"
#include "../../base/DescriptorHerpManager/SRVDescriptorHerpManager.h"

// コマンドリスト
ID3D12GraphicsCommandList* WorldTransform::sCommandList = nullptr;

WorldTransform::~WorldTransform()
{

}

void WorldTransform::Initialize()
{

	// 回転行列
	rotateMatrix_ = Matrix4x4::MakeRotateXYZMatrix(transform_.rotate);

	// 方向ベクトルで回転行列
	usedDirection_ = false;

	// スケールを考えない
	parentMatrix_ = Matrix4x4::MakeAffineMatrix(Vector3{ 1.0f,1.0f,1.0f }, transform_.rotate, transform_.translate);

	UpdateMatrix();

}

void WorldTransform::Initialize(const ModelNode& modelNode)
{

	// 回転行列
	rotateMatrix_ = Matrix4x4::MakeRotateXYZMatrix(transform_.rotate);

	// 方向ベクトルで回転行列
	usedDirection_ = false;

	// スケールを考えない
	parentMatrix_ = Matrix4x4::MakeAffineMatrix(Vector3{ 1.0f,1.0f,1.0f }, transform_.rotate, transform_.translate);

	//WVP用のリソースを作る。
	transformationMatrixBuff_ = BufferResource::CreateBufferResource(DirectXCommon::GetInstance()->GetDevice(), ((sizeof(TransformationMatrix) + 0xff) & ~0xff));
	//書き込むためのアドレスを取得
	transformationMatrixBuff_->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixMap_));

	transformationMatrixMap_->WVP = Matrix4x4::MakeIdentity4x4();
	transformationMatrixMap_->World = Matrix4x4::MakeIdentity4x4();
	transformationMatrixMap_->WorldInverseTranspose = Matrix4x4::MakeIdentity4x4();

	UpdateMatrix();

}

void WorldTransform::UpdateMatrix() {

	//拡大縮小行列
	Matrix4x4 scaleMatrix = Matrix4x4::MakeScaleMatrix(transform_.scale);
	// どう回転行列作るか
	if (usedDirection_) {
		// 回転行列
		rotateMatrix_ = Matrix4x4::DirectionToDirection(Vector3{0.0f,0.0f,1.0f}, direction_);
	}
	else {
		// 回転行列
		rotateMatrix_ = Matrix4x4::MakeRotateXYZMatrix(transform_.rotate);
	}

	//平行移動行列
	Matrix4x4 translateMatrix = Matrix4x4::MakeTranslateMatrix(transform_.translate);

	// ワールド行列
	worldMatrix_ = Matrix4x4::Multiply(scaleMatrix, Matrix4x4::Multiply(rotateMatrix_, translateMatrix));

	//拡大縮小行列
	scaleMatrix = Matrix4x4::MakeScaleMatrix(Vector3{ 1.0f,1.0f,1.0f });
	// 親子関係用
	parentMatrix_ = Matrix4x4::Multiply(scaleMatrix, Matrix4x4::Multiply(rotateMatrix_, translateMatrix));

	// 親子関係
	if (parent_) {
		worldMatrix_ = Matrix4x4::Multiply(worldMatrix_, parent_->parentMatrix_);
		parentMatrix_ = Matrix4x4::Multiply(parentMatrix_, parent_->parentMatrix_);
	}

}

void WorldTransform::UpdateMatrix(const Matrix4x4& rotateMatrix)
{

	//拡大縮小行列
	Matrix4x4 scaleMatrix = Matrix4x4::MakeScaleMatrix(transform_.scale);

	// 回転行列
	rotateMatrix_ = rotateMatrix;

	//平行移動行列
	Matrix4x4 translateMatrix = Matrix4x4::MakeTranslateMatrix(transform_.translate);

	// ワールド行列
	worldMatrix_ = Matrix4x4::Multiply(scaleMatrix, Matrix4x4::Multiply(rotateMatrix_, translateMatrix));

	//拡大縮小行列
	scaleMatrix = Matrix4x4::MakeScaleMatrix(Vector3{ 1.0f,1.0f,1.0f });
	// 親子関係用
	parentMatrix_ = Matrix4x4::Multiply(scaleMatrix, Matrix4x4::Multiply(rotateMatrix_, translateMatrix));

	// 親子関係
	if (parent_) {
		worldMatrix_ = Matrix4x4::Multiply(worldMatrix_, parent_->parentMatrix_);
		parentMatrix_ = Matrix4x4::Multiply(parentMatrix_, parent_->parentMatrix_);
	}

}

void WorldTransform::Map(const Matrix4x4& viewProjectionMatrix)
{

	transformationMatrixMap_->WVP = worldMatrix_ * viewProjectionMatrix;
	transformationMatrixMap_->World = worldMatrix_;
	transformationMatrixMap_->WorldInverseTranspose = Matrix4x4::Transpose(Matrix4x4::Inverse(worldMatrix_));

}

Vector3 WorldTransform::GetWorldPosition()
{

	Vector3 position;

	position.x = worldMatrix_.m[3][0];
	position.y = worldMatrix_.m[3][1];
	position.z = worldMatrix_.m[3][2];

	return position;

}

