#include "PlayerStateDash.h"
#include "../Player.h"
#include "../../../Engine/Math//DeltaTime.h"

void PlayerStateDash::Initialize()
{

	// プレイヤーはダッシュ状態に
	playerStateNo_ = kPlayerStateIndexDash;

	// 移動
	Vector3 move = { input_->GetLeftAnalogstick().x, 0.0f, 0.0f };
	// 横移動
	const float kMoveMagnificationX = 0.15f;
	moveVelocity_.x = Vector3::Normalize(move).x * kMoveMagnificationX;

	// フレームカウント
	frameCount_ = 0.0f;

	// モーション番号
	playerMotionNo_ = PlayerAnimation::PlayerMotionIndex::kPlayerMotionIndexDash;

}

void PlayerStateDash::Update()
{

	// ワールドトランスフォーム取得
	playerWorldTransform_ = player_->GetWorldTransformAdress();

	// 縦移動
	const float kDashSpeed_ = 2.0f;
	moveVelocity_.z = kDashSpeed_;

	// 座標変更
	playerWorldTransform_->transform_.translate += moveVelocity_;
	// 座標制御
	const float kTranslateMaxX = 9.0f;
	playerWorldTransform_->transform_.translate.x = std::clamp(playerWorldTransform_->transform_.translate.x, -kTranslateMaxX, kTranslateMaxX);

	// ワープ処理
	Warp();

	// 通常に移行
	frameCount_ += kDeltaTime_;
	const float kEndFrame = 0.15f;
	if (frameCount_ >= kEndFrame) {
		playerStateSystem_->SetReceiveCommand(true);
		playerStateNo_ = kPlayerStateIndexRoot;
		return;
	}

	// コマンドを受け取る
	playerStateSystem_->SetReceiveCommand(false);

}
