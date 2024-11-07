#include "TitleSceneObjectManager.h"
#include "../ObjectFactory.h"

void TitleSceneObjectManager::Initialize(LevelIndex levelIndex, LevelDataManager* levelDataManager)
{

	objectFactory_ = ObjectFactory::GetInstance();

	BaseObjectManager::Initialize(levelIndex, levelDataManager);

}
