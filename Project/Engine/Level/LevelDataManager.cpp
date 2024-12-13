#include "LevelDataManager.h"

void LevelDataManager::Initialize()
{

	// ローダーの初期化
	LevelDataLoader::Initialize();

	// データ格納
	for (uint32_t i = 0; i < LevelIndex::kLevelIndexOfCount; ++i) {
		levelDatas_[i].reset(LevelDataLoader::Load(kFileNames_[i]));
	}

	// 表示用クラスの初期化
	LevelDataViewing::Initialize(&levelDatas_, &kFileNames_);

}

void LevelDataManager::ImGuiDraw()
{

	LevelDataViewing::ImGuiViewing();

}
