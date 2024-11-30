#pragma once
#include "../../../Engine/Object/BaseObjectManager.h"
#include "../../System/Shadow/ShadowManager.h"

class Player;

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

	void LevelChange();

private: // メンバ関数

	/// <summary>
	/// 生成パターン
	/// </summary>
	/// <param name="levelIndex">レベル番号</param>
	/// <param name="levelDataManager">レベルデータマネージャー</param>
	void GeneratePattern(LevelIndex levelIndex, LevelDataManager* levelDataManager);

	/// <summary>
	/// 影更新
	/// </summary>
	void ShadowUpdate();

private: // メンバ変数

	// 影マネージャー
	std::unique_ptr<ShadowManager> shadowManager_;
	// 影モデル
	std::unique_ptr<Model> shadowModel_;

	// レベル
	uint32_t level_;

	// 生成タイミング削除位置
	uint32_t a_;

	// レベルデータマネージャー
	LevelDataManager* levelDataManager_;

	// プレイヤー
	Player* player_;

};

