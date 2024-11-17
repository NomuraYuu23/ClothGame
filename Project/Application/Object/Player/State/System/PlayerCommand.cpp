#include "PlayerCommand.h"
#include "../IPlayerState.h"

Input* PlayerCommand::input_ = Input::GetInstance();

PlayerCommand* PlayerCommand::GetInstance()
{
	static PlayerCommand instance;
	return &instance;
}

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

	return resultState;
}
