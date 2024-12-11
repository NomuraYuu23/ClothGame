#pragma once
#include "IPlayerState.h"

/// <summary>
/// プレイヤーのダメージ状態
/// </summary>
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

private: // メンバ変数

	// アニメーション時間
	float animationTime_;

};

