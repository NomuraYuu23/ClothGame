#pragma once
#include "GhostStateFactory.h"
#include "../IGhostState.h"

class Ghost;

class GhostStateSystem
{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="ghost">ゴースト/param>
	void Initialize(Ghost* ghost);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

public: // アクセッサ

	/// <summary>
	/// ステート取得
	/// </summary>
	/// <returns></returns>
	IGhostState* GetGhostState() { return ghostState_.get(); }

private: // メンバ変数

	// ステート
	std::unique_ptr<IGhostState> ghostState_;

	// 現在のステート番号
	uint32_t currentStateNo_;

	// 前のステート番号
	uint32_t prevStateNo_;

	// 次のステート番号
	uint32_t nextStateNo_;

	// ゴースト
	Ghost* ghost_;

};

