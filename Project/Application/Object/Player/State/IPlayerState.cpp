#include "IPlayerState.h"
#include "../Player.h"

uint32_t IPlayerState::playerStateNo_ = 0;

uint32_t IPlayerState::playerMotionNo_ = 0;

Player* IPlayerState::player_ = nullptr;

Input* IPlayerState::input_ = Input::GetInstance();

const float IPlayerState::kAutoMoveSpeed_ = 1.0f;

void IPlayerState::Move()
{

	// ワールドトランスフォーム取得
	playerWorldTransform_ = player_->GetWorldTransformAdress();
	
	// 移動
	Vector3 move = { input_->GetLeftAnalogstick().x, 0.0f, 0.0f };
	// 横移動
	moveVelocity_.x = Vector3::Normalize(move).x;
	// 縦移動
	moveVelocity_.z = kAutoMoveSpeed_;

	// 座標変更
	playerWorldTransform_->transform_.translate += moveVelocity_;

}
