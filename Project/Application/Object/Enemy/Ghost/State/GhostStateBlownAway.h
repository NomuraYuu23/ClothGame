#pragma once
#include "IGhostState.h"
class GhostStateBlownAway :
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

	// 速度
	Vector3 velocity_;

	// 加速度
	Vector3 acceleration_;

	// 経過時間
	float elapsedTime_;


};

