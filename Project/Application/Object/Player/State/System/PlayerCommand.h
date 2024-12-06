#pragma once
#include "../../../../../Engine/Input/Input.h"
class PlayerCommand
{

private: // 静的メンバ変数

	// ダッシュクールタイム
	static const float KDashCoolTime_;

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// コマンド
	/// </summary>
	/// <returns>ステート</returns>
	uint32_t Command();

	/// <summary>
	/// ダッシュ使用リセット
	/// </summary>
	void DashReset();

public: // アクセッサ

	/// <summary>
	/// ダッシュ経過時間取得
	/// </summary>
	/// <returns></returns>
	float GetDashElapsedTime() { return dashElapsedTime_; }

private: // メンバ変数

	// 入力
	Input* input_;

	// ダッシュ経過時間
	float dashElapsedTime_;

};

