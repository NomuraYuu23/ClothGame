#include "IGhostState.h"
#include "../Ghost.h"

uint32_t IGhostState::ghostStateNo_ = 0;

uint32_t IGhostState::ghostMotionNo_ = 0;

Ghost* IGhostState::ghost_ = nullptr;

GhostStateSystem* IGhostState::ghostStateSystem_ = nullptr;

Input* IGhostState::input_ = Input::GetInstance();
