#include "IPlayerState.h"
#include "../Player.h"

uint32_t IPlayerState::playerStateNo_ = 0;

uint32_t IPlayerState::playerMotionNo_ = 0;

Player* IPlayerState::player_ = nullptr;

Input* IPlayerState::input_ = Input::GetInstance();
