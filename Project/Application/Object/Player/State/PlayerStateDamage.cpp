#include "PlayerStateDamage.h"
#include "../Player.h"
#include "../../../../Engine/Math/DeltaTime.h"

void PlayerStateDamage::Initialize()
{

	// プレイヤーはジャンプ状態に
	playerStateNo_ = kPlayerStateIndexDamage;

	// モーション番号
	playerMotionNo_ = PlayerAnimation::PlayerMotionIndex::kPlayerMotionIndexDamage;

	// アニメーション時間
	animationTime_ = 0.0f;

}

void PlayerStateDamage::Update()
{

	// アニメーションの終了時間
	const float kAnimationEndTime = 0.95f;

	// ダメージアニメーション中
	animationTime_ += kDeltaTime_;

	// アニメーション終了後ワープ 
	if (kAnimationEndTime <= animationTime_) {
		// ワールドトランスフォーム取得
		playerWorldTransform_ = player_->GetWorldTransformAdress();

		// ワープ
		const Vector3 kWarpTranslate = { 0.0f, 4.0f, -470.0f };
		playerWorldTransform_->transform_.translate = kWarpTranslate;
		player_->SetWarping(true);

		// 状態をRootへ
		playerStateNo_ = kPlayerStateIndexRoot;
	}

	// コマンドを受け取らない
	playerStateSystem_->SetReceiveCommand(false);

}
