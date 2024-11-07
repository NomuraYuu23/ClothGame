#include "OneOfManyObjects.h"

void OneOfManyObjects::Initialize()
{

	//ワールド行列
	worldMatrix_ = Matrix4x4::MakeIdentity4x4();

	// 回転行列
	rotateMatrix_ = Matrix4x4::MakeRotateXYZMatrix(transform_.rotate);

	// 方向ベクトルで回転行列
	usedDirection_ = false;

	// 死んでいるか
	isDead_ = false;

	//	マテリアルデータ
	materialData_.color = { 1.0f, 1.0f, 1.0f, 1.0f };
	materialData_.enableLighting = HalfLambert;
	materialData_.shininess = 100.0f;
	materialData_.uvTransform = Matrix4x4::MakeAffineMatrix(Vector3{ 1.0f,1.0f,1.0f }, Vector3{ 0.0f,0.0f,0.0f }, Vector3{ 0.0f,0.0f,0.0f });
	materialData_.environmentCoefficient = 0.0f;


}

void OneOfManyObjects::Update()
{

	//拡大縮小行列
	Matrix4x4 scaleMatrix = Matrix4x4::MakeScaleMatrix(transform_.scale);
	// どう回転行列作るか
	if (usedDirection_) {
		// 回転行列
		rotateMatrix_ = Matrix4x4::DirectionToDirection(Vector3{ 0.0f,0.0f,1.0f }, direction_);
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
