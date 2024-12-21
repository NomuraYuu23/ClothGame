#include "ClothGate.h"
#include "../Player/Player.h"
#include "../../../Engine/Math/RandomEngine.h"
#include "../../../Engine/Math/Ease.h"
#include "../../../Engine/Math/DeltaTime.h"

const Vector2 ClothGate::kClothScale_ = { 4.0f,3.0f };

const Vector2 ClothGate::kClothDiv_ = { 8.0f,8.0f };

const Vector3 ClothGate::kBaseLeftFixed_ = { -2.0f, 3.0f, 0.0f };

const Vector3 ClothGate::kBaseRightFixed_ = { 2.0f, 3.0f, 0.0f };

void ClothGate::Initialize(LevelData::MeshData* data)
{

	// ベース部分
	BaseClothGate::Initialize(data);

	// 布
	ClothInitialize(kClothScale_, kClothDiv_);

}

void ClothGate::ClothReset()
{

	// 位置をリセット
	Vector3 resetPosition = { 0.0f,0.0f,0.0f };
	for (uint32_t y = 0; y <= static_cast<uint32_t>(kClothDiv_.y); ++y) {
		for (uint32_t x = 0; x <= static_cast<uint32_t>(kClothDiv_.x); ++x) {
			// 重み
			cloth_->SetWeight(y, x, true);
			// 位置
			resetPosition = worldTransform_.GetWorldPosition();
			resetPosition.x += Ease::Easing(Ease::EaseName::Lerp, kBaseLeftFixed_.x, kBaseRightFixed_.x, static_cast<float>(x) / kClothDiv_.x);
			resetPosition.y += Ease::Easing(Ease::EaseName::Lerp, kBaseRightFixed_.y, 0.0f, static_cast<float>(y) / kClothDiv_.y);
			cloth_->SetPosition(y, x, resetPosition);
		}
	}

}

void ClothGate::ClothUpdate()
{

	// ベース部分
	BaseClothGate::ClothUpdate();

	// 固定部分

	// 右質点位置
	const uint32_t kRightX = static_cast<uint32_t>(kClothDiv_.x);

	// 座標
	const Vector3 kLeftTopPosition = worldTransform_.GetWorldPosition() + kBaseLeftFixed_;
	const Vector3 kRightTopPosition = worldTransform_.GetWorldPosition() + kBaseRightFixed_;

	// 設定
	cloth_->SetWeight(0, 0, false);
	cloth_->SetPosition(0, 0, kLeftTopPosition);
	cloth_->SetWeight(0, kRightX, false);
	cloth_->SetPosition(0, kRightX, kRightTopPosition);

}

void ClothGate::ImGuiDraw()
{

}
