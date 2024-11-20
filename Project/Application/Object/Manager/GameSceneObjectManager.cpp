#include "GameSceneObjectManager.h"
#include "../Factory/ObjectFactory.h"
#include "../ClothGate/ClothGate.h"

void GameSceneObjectManager::Initialize(LevelIndex levelIndex, LevelDataManager* levelDataManager)
{

	// オブジェクトファクトリー
	objectFactory_ = std::make_unique<ObjectFactory>();
	static_cast<ObjectFactory*>(objectFactory_.get())->Initialize(this);
	
	BaseObjectManager::Initialize(levelIndex, levelDataManager);


	//for (uint32_t i = 0; i < 8; i++) {
	//	GeneratePattern(kLevelIndexGenerationPattern_00, levelDataManager, i);
	//}

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
	// パターンの始まる距離
	const float kPatternStartPosition = -400.0f;
	// パターン区間の大きさ
	const float kPatternSize = 100.0f;

	// レベルデータのオブジェクトを走査
	for (std::vector<LevelData::ObjectData>::iterator it = levelData->objectsData_.begin();
		it != levelData->objectsData_.end(); ++it) {

		// オブジェクトの参照
		LevelData::ObjectData objectData = *it;

		// 型にあわせてInitialize
		std::unique_ptr<IObject> object;
		object.reset(objectFactory_->CreateObject(objectData));

		if (object) {

			// パターン回数に応じて移動
			object->GetWorldTransformAdress()->transform_.translate.z += currentGenerationCount * kPatternSize + kPatternStartPosition;
			object->GetWorldTransformAdress()->UpdateMatrix();
			static_cast<ClothGate*>(object.get())->ClothReset();

			// listへ
			objects_.emplace_back(object->GetName(), std::move(object));
		}

	}
}
