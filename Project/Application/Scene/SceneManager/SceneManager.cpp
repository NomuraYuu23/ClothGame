#include "SceneManager.h"
#include "../SceneFactory/SceneFactory.h"
#include "../../SceneTransition/SceneTransitionFactory/SceneTransitionFactory.h"

void SceneManager::Initialize(uint32_t earlySceneNo) {

	// シーンファクトリー
	sceneFactory_ = SceneFactory::GetInstance();
	// シーン遷移ファクトリー
	sceneTransitionFactory_ = SceneTransitionFactory::GetInstance();

	// ベースの初期化
	BaseSceneManager::Initialize(earlySceneNo);

}