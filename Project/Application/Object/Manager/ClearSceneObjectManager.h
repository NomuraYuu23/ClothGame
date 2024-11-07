#pragma once
#include "../../../Engine/Object/BaseObjectManager.h"
class ClearSceneObjectManager :
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

