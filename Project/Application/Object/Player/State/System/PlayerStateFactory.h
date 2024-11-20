#pragma once
#include "../IPlayerState.h"

class PlayerStateFactory
{

public: // メンバ関数

	// シーン生成
	static IPlayerState* CreatePlayerState(uint32_t playerStateName);

};

