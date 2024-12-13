#include "PlayerCommand.h"
#include "../IPlayerState.h"
#include "../../../../../Engine/Math/DeltaTime.h"

const float PlayerCommand::KDashCoolTime_ = 0.5f;

void PlayerCommand::Initialize()
{

	input_ = Input::GetInstance();

	dashElapsedTime_ = KDashCoolTime_;

}

void PlayerCommand::Update()
{

	dashElapsedTime_ += kDeltaTime_;
	if (dashElapsedTime_ >= KDashCoolTime_) {
		dashElapsedTime_ = KDashCoolTime_;
	}

}

uint32_t PlayerCommand::Command()
{
	
	uint32_t resultState = PlayerStateIndex::kPlayerStateIndexRoot;

	// Aボタンが押されていたらジャンプ
	if (input_->TriggerJoystick(JoystickButton::kJoystickButtonA)) {
		resultState = kPlayerStateIndexJump;
	}
	// Bボタンが押されていたらダッシュ
	else if (input_->TriggerJoystick(JoystickButton::kJoystickButtonB) && dashElapsedTime_ == KDashCoolTime_) {
		resultState = kPlayerStateIndexDash;
		dashElapsedTime_ = 0.0f;
	}

	return resultState;
}

void PlayerCommand::DashReset()
{

	// 経過時間を最大に
	dashElapsedTime_ = KDashCoolTime_;

}
