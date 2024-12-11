#pragma once
#include "IGhostState.h"

/// <summary>
/// ゴーストの通常状態
/// </summary>
class GhostStateRoot :
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

};

