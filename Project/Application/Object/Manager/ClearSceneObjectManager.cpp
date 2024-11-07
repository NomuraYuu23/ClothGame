#include "ClearSceneObjectManager.h"
#include "../ObjectFactory.h"

void ClearSceneObjectManager::Initialize(LevelIndex levelIndex, LevelDataManager* levelDataManager)
{

	objectFactory_ = ObjectFactory::GetInstance();

	BaseObjectManager::Initialize(levelIndex, levelDataManager);

}
