#include "PlayerStateJump.h"
#include "../Player.h"
#include "../../../Engine/Physics/Gravity/Gravity.h"

void PlayerStateJump::Initialize()
{

	// ジャンプ初速を入れる
	const float kJumpInitialSpeed = 1.2f;
	moveVelocity_.y = kJumpInitialSpeed;

	// プレイヤーは浮く
	player_->SetFloating(true);

	// プレイヤーはジャンプ状態に
	playerStateNo_ = kPlayerStateIndexJump;

}

void PlayerStateJump::Update()
{

	// 重力で速度を変更する
	moveVelocity_ += Gravity::Execute() * 0.5f;
	// 速度制限
	const float kJumpSpeedMin = -2.0f;
	if (moveVelocity_.y < -2.0f) {
		moveVelocity_.y = -2.0f;
	}

	// 移動関数
	Move();

	// 地面に触れたらステートをRootに
	if (!player_->GetFloating()) {
		playerStateNo_ = kPlayerStateIndexRoot;
	}

	// コマンドを受け取らない
	playerStateSystem_->SetReceiveCommand(false);

}
