#pragma once
#include "IGhostState.h"

/// <summary>
/// ゴーストの横移動状態
/// </summary>
class GhostStateSideMove :
    public IGhostState
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

	// ラジアン角
	float radian_;

};

