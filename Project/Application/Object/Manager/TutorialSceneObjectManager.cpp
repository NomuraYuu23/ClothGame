#include "TutorialSceneObjectManager.h"
#include "../ObjectFactory.h"

void TutorialSceneObjectManager::Initialize(LevelIndex levelIndex, LevelDataManager* levelDataManager)
{

	objectFactory_ = ObjectFactory::GetInstance();

	BaseObjectManager::Initialize(levelIndex, levelDataManager);

}
