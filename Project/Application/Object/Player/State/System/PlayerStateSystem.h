#pragma once
#include <cstdint>
#include <memory>
#include "PlayerStateFactory.h"
#include "../IPlayerState.h"
#include "PlayerCommand.h"

class Player;

class PlayerStateSystem
{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="player">プレイヤー</param>
	void Initialize(Player* player);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

public: // アクセッサ

	/// <summary>
	/// ステート取得
	/// </summary>
	/// <returns></returns>
	IPlayerState* GetPlayerState() { return playerState_.get(); }

private: // メンバ変数

	// ステート
	std::unique_ptr<IPlayerState> playerState_;

	// 現在のステート番号
	uint32_t currentStateNo_;

	// 前のステート番号
	uint32_t prevStateNo_;

	// 次のステート番号
	uint32_t nextStateNo_;

	// ステートファクトリー
	PlayerStateFactory* playerStateFactory_;

	// コマンド
	PlayerCommand* playerCommand_;

	// コマンドを受け取るか
	bool receiveCommand_;

	// 割り込みコマンドがあるか
	bool interruptCommand_;

};

