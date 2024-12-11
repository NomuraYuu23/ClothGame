#pragma once
#include "IPlayerState.h"

/// <summary>
/// プレイヤーのジャンプ状態
/// </summary>
class PlayerStateJump :
    public IPlayerState
{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

};

