#include "SceneTransitionFactory.h"
#include "../../../Engine/Scene/IScene/IScene.h"
#include "../SceneTransitionBlackOut/SceneTransitionBlackOut.h"
#include "../SceneTransitionCurtain/SceneTransitionCurtain.h"

SceneTransitionFactory* SceneTransitionFactory::GetInstance()
{
	static SceneTransitionFactory instance;
	return &instance;
}

ISceneTransition* SceneTransitionFactory::CreateSceneTransition(int sceneName, int requestSeneName)
{

	ISceneTransition* newSceneTransition = nullptr;

	// タイトルからゲーム
	if (sceneName == SceneName::kTitle && requestSeneName == SceneName::kGame) {
		newSceneTransition = new SceneTransitionCurtain();
	}
	else {
		// 現在のシーンから次のシーンへ行くときの遷移が設定されてない
		newSceneTransition = new SceneTransitionCurtain();
	}

	return newSceneTransition;
}
