#include "GameSceneObjectManager.h"
#include "../ObjectFactory.h"

void GameSceneObjectManager::Initialize(LevelIndex levelIndex, LevelDataManager* levelDataManager)
{

	objectFactory_ = ObjectFactory::GetInstance();

	BaseObjectManager::Initialize(levelIndex, levelDataManager);

}
