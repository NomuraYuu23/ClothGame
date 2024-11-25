#include "PlayerStateFall.h"
#include "../Player.h"
#include "../../../Engine/Physics/Gravity/Gravity.h"

void PlayerStateFall::Initialize()
{

	// 速度を一度0へ
	moveVelocity_.y = 0.0f;

	// プレイヤーは浮く
	player_->SetFloating(true);

	// プレイヤーは落下状態に
	playerStateNo_ = kPlayerStateIndexFall;

}

void PlayerStateFall::Update()
{	
	
	// 重力で速度を変更する
	moveVelocity_ += Gravity::Execute() * 0.5f;
	// 速度制限
	const float kJumpSpeedMin = -2.0f;
	if (moveVelocity_.y < kJumpSpeedMin) {
		moveVelocity_.y = kJumpSpeedMin;
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
