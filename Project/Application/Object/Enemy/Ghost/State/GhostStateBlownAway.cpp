#include "GhostStateBlownAway.h"
#include "../Ghost.h"
#include "../../../../../Engine/Math/DeltaTime.h"
#include "../../../../../Engine/Physics/Gravity/Gravity.h"

void GhostStateBlownAway::Initialize()
{

	ghostStateNo_ = kGhostStateIndexBlownAway;

	const Vector3 kInitVelocity = { 0.0f, 1.2f, 1.0f};
	velocity_ = kInitVelocity;

	const Vector3 kInitAcceleration = { 0.0f, Gravity::Execute().y * 0.5f, 1.0f};
	acceleration_ = kInitAcceleration;

}

void GhostStateBlownAway::Update()
{

	// ワールドトランスフォーム取得
	WorldTransform* worldTransform = ghost_->GetWorldTransformAdress();

	// 速度
	velocity_ += acceleration_;

	// 移動
	worldTransform->transform_.translate += velocity_;

	// 生存時間
	const float kLifeTime = 2.0f;
	elapsedTime_ += kDeltaTime_;
	if (elapsedTime_ >= kLifeTime) {
		elapsedTime_ = kLifeTime;
		ghost_->SetIsDead(true);
	}

}
