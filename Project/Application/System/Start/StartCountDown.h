#pragma once
#include "../../../Engine/2D/Sprite.h"

/// <summary>
/// スタートカウントダウンシステム
/// </summary>
class StartCountDown
{

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
	/// 描画
	/// </summary>
	void Draw();

public: // アクセッサ

	/// <summary>
	/// カウントダウン終了したか
	/// </summary>
	/// <returns></returns>
	bool GetCountdownEnded() { return countdownEnded_; }

private: // メンバ変数

	// 数字テクスチャハンドル
	uint32_t numTextureHnadle_;

	// 数字スプライト
	std::unique_ptr<Sprite> numSprite_;

	// startテクスチャハンドル
	uint32_t startTextureHnadle_;

	// startスプライト
	std::unique_ptr<Sprite> startSprite_;

	// 説明テクスチャハンドル
	uint32_t explanationTextureHnadle_;

	// 説明スプライト
	std::unique_ptr<Sprite> explanationSprite_;

	// カウントダウン秒数
	float countDownTime_;

	// α値
	float alpha_;

	// カウントダウン終了したか
	bool countdownEnded_;

	// システム終了したか
	bool systemEnded_;

};

