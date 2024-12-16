#pragma once
#include "IPlayerState.h"

/// <summary>
/// プレイヤーのダッシュ状態
/// </summary>
class PlayerStateDash :
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

	// フレームカウント
	float frameCount_;

};

