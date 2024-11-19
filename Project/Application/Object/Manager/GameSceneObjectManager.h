#pragma once
#include "../../../Engine/Object/BaseObjectManager.h"
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

private: // メンバ関数

	/// <summary>
	/// 生成パターン
	/// </summary>
	/// <param name="levelIndex">レベル番号</param>
	/// <param name="levelDataManager">レベルデータマネージャー</param>
	/// <param name="currentGenerationCount">現在の生成回数</param>
	void GeneratePattern(LevelIndex levelIndex, LevelDataManager* levelDataManager, uint32_t currentGenerationCount);

};

