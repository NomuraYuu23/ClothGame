#pragma once
#include "IPlayerState.h"

/// <summary>
/// プレイヤーの通常状態
/// </summary>
class PlayerStateRoot :
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

