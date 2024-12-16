#pragma once
#include "../IPlayerState.h"

/// <summary>
/// プレイヤーの状態ファクトリー
/// </summary>
class PlayerStateFactory
{

public: // メンバ関数

	// シーン生成
	static IPlayerState* CreatePlayerState(uint32_t playerStateName);

};

