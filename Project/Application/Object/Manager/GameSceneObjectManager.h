#pragma once
#include "../../../Engine/Object/BaseObjectManager.h"
#include "../../System/Shadow/ShadowManager.h"
class GameSceneObjectManager :
    public BaseObjectManager
{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="levelIndex">レベル番号</param>
	/// <param name="levelDataManager">レベルデータマネージャー</param>
	void Initialize(LevelIndex levelIndex, LevelDataManager* levelDataManager) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera">カメラ</param>
	void Draw(BaseCamera& camera) override;

	/// <summary>
	/// 描画(デバッグ)
	/// </summary>
	/// <param name="camera">カメラ</param>
	/// <param name="drawLine">線描画クラス</param>
	void Draw(BaseCamera& camera, DrawLine* drawLine) override;

private: // メンバ関数

	/// <summary>
	/// 生成パターン
	/// </summary>
	/// <param name="levelIndex">レベル番号</param>
	/// <param name="levelDataManager">レベルデータマネージャー</param>
	/// <param name="currentGenerationCount">現在の生成回数</param>
	void GeneratePattern(LevelIndex levelIndex, LevelDataManager* levelDataManager, uint32_t currentGenerationCount);


	void ShadowUpdate();

private: // メンバ変数

	// 影マネージャー
	std::unique_ptr<ShadowManager> shadowManager_;
	// 影モデル
	std::unique_ptr<Model> shadowModel_;

};

