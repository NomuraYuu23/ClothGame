#include "BigClothGate.h"
#include "../../../Engine/Math/Ease.h"

const Vector2 BigClothGate::kClothScale_ = { 6.0f, 5.0f };

const Vector2 BigClothGate::kClothDiv_ = { 8.0f,8.0f };

const Vector3 BigClothGate::kBaseLeftFixed_ = { 0.0f, 5.0f, -3.0f };

const Vector3 BigClothGate::kBaseRightFixed_ = { 0.0f, 5.0f, 3.0f };

void BigClothGate::Initialize(LevelData::MeshData* data)
{

	// ベース部分
	BaseClothGate::Initialize(data);

	// ワールド座標からの左固定部分
	leftFixed_ = Matrix4x4::TransformNormal(kBaseLeftFixed_, worldTransform_.worldMatrix_);
	// ワールド座標からの右固定部分
	rightFixed_ = Matrix4x4::TransformNormal(kBaseRightFixed_, worldTransform_.worldMatrix_);

	// 布
	ClothInitialize(kClothScale_, kClothDiv_);

}

void BigClothGate::ClothReset()
{

	// 位置をリセット
	Vector3 resetPosition = { 0.0f,0.0f,0.0f };
	for (uint32_t y = 0; y <= static_cast<uint32_t>(kClothDiv_.y); ++y) {
		for (uint32_t x = 0; x <= static_cast<uint32_t>(kClothDiv_.x); ++x) {
			// 重み
			cloth_->SetWeight(y, x, true);
			// 位置
			resetPosition = worldTransform_.GetWorldPosition();
			resetPosition.x += Ease::Easing(Ease::EaseName::Lerp, leftFixed_.x, rightFixed_.x, static_cast<float>(x) / kClothDiv_.x);
			resetPosition.y += Ease::Easing(Ease::EaseName::Lerp, rightFixed_.y, 0.0f, static_cast<float>(y) / kClothDiv_.y);
			cloth_->SetPosition(y, x, resetPosition);
		}
	}

}

void BigClothGate::ClothUpdate()
{

	// ベース部分
	BaseClothGate::ClothUpdate();

	// 固定部分

	// 右質点位置
	const uint32_t kRightX = static_cast<uint32_t>(kClothDiv_.x);

	// 座標
	const Vector3 kLeftTopPosition = worldTransform_.GetWorldPosition() + leftFixed_;
	const Vector3 kRightTopPosition = worldTransform_.GetWorldPosition() + rightFixed_;

	// 設定
	cloth_->SetWeight(0, 0, false);
	cloth_->SetPosition(0, 0, kLeftTopPosition);
	cloth_->SetWeight(0, kRightX, false);
	cloth_->SetPosition(0, kRightX, kRightTopPosition);

}
