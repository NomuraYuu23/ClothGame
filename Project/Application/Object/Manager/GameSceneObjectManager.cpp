#include "GameSceneObjectManager.h"
#include "../Factory/ObjectFactory.h"

void GameSceneObjectManager::Initialize(LevelIndex levelIndex, LevelDataManager* levelDataManager)
{

	// オブジェクトファクトリー
	objectFactory_ = std::make_unique<ObjectFactory>();
	static_cast<ObjectFactory*>(objectFactory_.get())->Initialize(this);
	
	BaseObjectManager::Initialize(levelIndex, levelDataManager);

}
