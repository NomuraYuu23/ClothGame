#pragma once
#include "../IGhostState.h"

/// <summary>
/// ゴースト状態用ファクトリー
/// </summary>
class GhostStateFactory
{

public: // メンバ関数

	// シーン生成
	static IGhostState* CreateGhostState(uint32_t ghostStateName);

};

