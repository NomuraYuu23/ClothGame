#pragma once
#include <memory>
#include "../../Light/DirectionalLight/DirectionalLight.h"
#include "../../Light/PointLight/PointLightManager.h"
#include "../../Light/SpotLight/SpotLightManager.h"

/// <summary>
/// シーン遷移
/// </summary>
class ISceneTransition
{

public: // メンバ関数

	/// <summary>
	/// デストラクタ
	/// </summary>
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

	/// <summary>
	/// 遷移しているか取得
	/// </summary>
	/// <returns></returns>
	bool GetTransitioning() { return transitioning_; }

	/// <summary>
	/// 遷移しているか設定
	/// </summary>
	/// <returns></returns>
	void SetTransitioning(bool transitioning) { transitioning_ = transitioning; }

	/// <summary>
	/// シーンを切り替えるか取得
	/// </summary>
	/// <returns></returns>
	bool GetSwitchScene() { return switchScene_; }

	/// <summary>
	/// シーンを切り替えるか設定
	/// </summary>
	/// <param name="switchScene"></param>
	void SetSwitchScene(bool switchScene) { switchScene_ = switchScene; }

	/// <summary>
	/// 更新を停止する取得
	/// </summary>
	/// <returns></returns>
	bool GetStoppingUpdates() { return stoppingUpdates_; }

	/// <summary>
	/// 更新を停止する設定
	/// </summary>
	/// <param name="stoppingUpdates"></param>
	void SetStoppingUpdates(bool stoppingUpdates) { stoppingUpdates_ = stoppingUpdates; }

	/// <summary>
	/// フェードインか設定
	/// </summary>
	/// <param name="isFadeIn"></param>
	void SetIsFadeIn(bool isFadeIn) { isFadeIn_ = isFadeIn; }

protected: // メンバ変数

	// フェード時間
	float fadeInTime_; //フェードインタイム
	float fadeOutTime_; //フェードアウトタイム
	float fadeTimer_; // タイマー

	bool transitioning_; //遷移しているか
	bool isFadeIn_; // フェードインか
	bool switchScene_; //シーンを切り替えるか

	bool stoppingUpdates_; // 更新を停止する


	// 平行光源
	std::unique_ptr<DirectionalLight> directionalLight_;
	// 平行光源データ
	DirectionalLightData directionalLightData_;

	// 点光源
	std::unique_ptr<PointLightManager> pointLightManager_;
	// 点光源データ
	std::array<PointLightData, PointLightManager::kNumInstanceMax_> pointLightDatas_;

	// スポットライト
	std::unique_ptr<SpotLightManager> spotLightManager_;
	// スポットライトデータ
	std::array<SpotLightData, SpotLightManager::kNumInstanceMax_> spotLightDatas_;

};

