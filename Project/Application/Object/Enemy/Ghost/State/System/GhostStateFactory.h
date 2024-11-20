#pragma once
#include "../IGhostState.h"

class GhostStateFactory
{

public: // メンバ関数

	// シーン生成
	static IGhostState* CreateGhostState(uint32_t ghostStateName);

};

