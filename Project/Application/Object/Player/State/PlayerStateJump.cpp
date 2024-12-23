#include "PlayerStateJump.h"
#include "../Player.h"
#include "../../../Engine/Physics/Gravity/Gravity.h"

void PlayerStateJump::Initialize()
{

	// ジャンプ初速を入れる
	const float kJumpInitialSpeed = 0.8f;
	moveVelocity_.y = kJumpInitialSpeed;

	// プレイヤーは浮く
	player_->SetFloating(true);

	// プレイヤーはジャンプ状態に
	playerStateNo_ = kPlayerStateIndexJump;

	// モーション番号
	playerMotionNo_ = PlayerAnimation::PlayerMotionIndex::kPlayerMotionIndexJump;

}

void PlayerStateJump::Update()
{

	// 重力で速度を変更する
	// 重力倍率
	const float kGravityMagnification = 0.2f;
	moveVelocity_ += Gravity::Execute() * kGravityMagnification;
	// 速度制限
	const float kJumpSpeedMin = 0.0f;
	// 落下状態へ
	if (moveVelocity_.y < kJumpSpeedMin) {
		moveVelocity_.y = kJumpSpeedMin;
		playerStateNo_ = kPlayerStateIndexFall;
	}

	// 移動関数
	Move();

	// コマンドを受け取らない
	playerStateSystem_->SetReceiveCommand(false);

}
