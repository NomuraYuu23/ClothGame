#include "GameSceneObjectManager.h"
#include "../Factory/ObjectFactory.h"
#include "../ClothGate/ClothGate.h"

void GameSceneObjectManager::Initialize(LevelIndex levelIndex, LevelDataManager* levelDataManager)
{

	// オブジェクトファクトリー
	objectFactory_ = std::make_unique<ObjectFactory>();
	static_cast<ObjectFactory*>(objectFactory_.get())->Initialize(this);
	
	BaseObjectManager::Initialize(levelIndex, levelDataManager);


	// お試し
	for (uint32_t i = 0; i < 8; i++) {
		GeneratePattern(kLevelIndexGenerationPattern_00, levelDataManager, i);
	}

}

void GameSceneObjectManager::Update()
{

	BaseObjectManager::Update();

	// ワープ時、オブジェクトの追加


}

void GameSceneObjectManager::GeneratePattern(LevelIndex levelIndex, LevelDataManager* levelDataManager, uint32_t currentGenerationCount)
{

	// レベルデータの取得
	LevelData* levelData = levelDataManager_->GetLevelDatas(levelIndex);

	// レベルデータのオブジェクトを走査
	for (std::vector<LevelData::ObjectData>::iterator it = levelData->objectsData_.begin();
		it != levelData->objectsData_.end(); ++it) {

		// オブジェクトの参照
		LevelData::ObjectData objectData = *it;

		// 型にあわせてInitialize
		std::unique_ptr<IObject> object;
		object.reset(static_cast<ObjectFactory*>(objectFactory_.get())->CreateObjectPattern(objectData, currentGenerationCount));

		if (object) {
			// listへ
			objects_.emplace_back(object->GetName(), std::move(object));
		}

	}
}
