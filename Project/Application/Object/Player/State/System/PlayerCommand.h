#pragma once
#include "../../../../../Engine/Input/Input.h"
class PlayerCommand
{

public: // 静的メンバ関数

	/// <summary>
	/// インスタンス取得
	/// </summary>
	/// <returns></returns>
	static PlayerCommand* GetInstance();

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// コマンド
	/// </summary>
	/// <returns>ステート</returns>
	uint32_t Command();

private: // メンバ変数

	// 入力
	static Input* input_;

private:
	PlayerCommand() = default;
	~PlayerCommand() = default;
	PlayerCommand(const PlayerCommand&) = delete;
	const PlayerCommand& operator=(const PlayerCommand&) = delete;

};

