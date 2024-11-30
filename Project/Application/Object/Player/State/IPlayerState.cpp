#include "IPlayerState.h"
#include "../Player.h"

uint32_t IPlayerState::playerStateNo_ = 0;

uint32_t IPlayerState::playerMotionNo_ = 0;

Player* IPlayerState::player_ = nullptr;

PlayerStateSystem* IPlayerState::playerStateSystem_ = nullptr;

Input* IPlayerState::input_ = Input::GetInstance();

const float IPlayerState::kAutoMoveSpeed_ = 0.6f;

void IPlayerState::Move()
{

	// ワールドトランスフォーム取得
	playerWorldTransform_ = player_->GetWorldTransformAdress();
	
	// 移動
	Vector3 move = { input_->GetLeftAnalogstick().x, 0.0f, 0.0f };
	// 横移動
	const float kMoveMagnificationX = 0.15f;
	moveVelocity_.x = Vector3::Normalize(move).x * kMoveMagnificationX;
	// 縦移動
	moveVelocity_.z = kAutoMoveSpeed_;

	// 座標変更
	playerWorldTransform_->transform_.translate += moveVelocity_;
	// 座標制御
	const float kTranslateMaxX = 9.0f;
	playerWorldTransform_->transform_.translate.x = std::clamp(playerWorldTransform_->transform_.translate.x, -kTranslateMaxX, kTranslateMaxX);

	// ワープ処理
	Warp();

}

void IPlayerState::Warp()
{

	//位置確認
	const float kTranslateMaxZ = 400.0f;
	if (kTranslateMaxZ <= playerWorldTransform_->transform_.translate.z) {

		// ワープ
		const float kWarpTranslateZ = -470.0f;
		playerWorldTransform_->transform_.translate.z = kWarpTranslateZ;
		player_->SetWarping(true);

	}

}
