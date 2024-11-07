#pragma once
#include <memory>

/// <summary>
/// シーン遷移
/// </summary>
class ISceneTransition
{

public: // メンバ関数

	virtual ~ISceneTransition();

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() = 0;

public: // アクセッサ

	bool GetTransitioning() { return transitioning_; }

	void SetTransitioning(bool transitioning) { transitioning_ = transitioning; }

	bool GetSwitchScene() { return switchScene_; }

	void SetSwitchScene(bool switchScene) { switchScene_ = switchScene; }

	bool GetStoppingUpdates() { return stoppingUpdates_; }

	void SetStoppingUpdates(bool stoppingUpdates) { stoppingUpdates_ = stoppingUpdates; }

	void SetIsFadeIn(bool isFadeIn) { isFadeIn_ = isFadeIn; }

protected: // メンバ変数

	// フェード時間
	float fadeInTime_; //フェードインタイム
	float fadeOutTime_; //フェードアウトタイム
	float fadeTimer_; // タイマー

	bool transitioning_; //遷移しているか
	bool isFadeIn_; // フェードインか
	bool switchScene_; //シーンを切り替える

	bool stoppingUpdates_; // 更新を停止する

};

