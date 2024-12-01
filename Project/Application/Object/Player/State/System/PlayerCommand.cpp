#include "PlayerCommand.h"
#include "../IPlayerState.h"

Input* PlayerCommand::input_ = Input::GetInstance();

void PlayerCommand::Initialize()
{
}

uint32_t PlayerCommand::Command()
{
	
	uint32_t resultState = PlayerStateIndex::kPlayerStateIndexRoot;

	// Aボタンが押されていたらジャンプ
	if (input_->TriggerJoystick(JoystickButton::kJoystickButtonA)) {
		resultState = kPlayerStateIndexJump;
	}
	// Bボタンが押されていたらダッシュ
	else if (input_->TriggerJoystick(JoystickButton::kJoystickButtonB)) {
		resultState = kPlayerStateIndexDash;
	}

	return resultState;
}
