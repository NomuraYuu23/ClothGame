#pragma once
#include "../../../Engine/Object/BaseObjectManager.h"

/// <summary>
/// チュートリアルシーン用のオブジェクトマネージャー
/// </summary>
class TutorialSceneObjectManager :
    public BaseObjectManager
{

public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="levelIndex">レベル番号</param>
	/// <param name="levelDataManager">レベルデータマネージャー</param>
	virtual void Initialize(LevelIndex levelIndex, LevelDataManager* levelDataManager) override;

};

