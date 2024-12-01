#pragma once
#include "../../../../../Engine/Input/Input.h"
class PlayerCommand
{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	static void Initialize();

	/// <summary>
	/// コマンド
	/// </summary>
	/// <returns>ステート</returns>
	static uint32_t Command();

private: // メンバ変数

	// 入力
	static Input* input_;

};

