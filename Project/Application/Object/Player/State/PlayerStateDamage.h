#pragma once
#include "IPlayerState.h"
class PlayerStateDamage :
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

