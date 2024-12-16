#pragma once
#include "GhostStateFactory.h"
#include "../IGhostState.h"

// 前方宣言
class Ghost;

/// <summary>
/// ゴーストの状態システム
/// </summary>
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

	/// <summary>
	/// 次のステート番号取得
	/// </summary>
	/// <param name="nextStateNo">次のステート番号</param>
	void SetNextStateNo(uint32_t nextStateNo) { nextStateNo_ = nextStateNo; }

	/// <summary>
	/// 割り込みコマンドがあるか取得
	/// </summary>
	/// <param name="interruptCommand">割り込みコマンドがあるか</param>
	void SetInterruptCommand(bool interruptCommand) { interruptCommand_ = interruptCommand; }

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

	// 割り込みコマンドがあるか
	bool interruptCommand_;

};

