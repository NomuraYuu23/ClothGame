#pragma once
#include "../../../Engine/SceneTransition/SceneTransitionFactory/AbstractSceneTransitionFactory.h"

class SceneTransitionFactory : public AbstractSceneTransitionFactory{

public: // 静的メンバ関数

	/// <summary>
	/// インスタンス取得
	/// </summary>
	/// <returns></returns>
	static SceneTransitionFactory* GetInstance();

public: // メンバ関数

	// シーン生成
	ISceneTransition* CreateSceneTransition(int sceneName, int requestSeneName) override;

private:
	SceneTransitionFactory() = default;
	~SceneTransitionFactory() = default;
	SceneTransitionFactory(const SceneTransitionFactory&) = delete;
	const SceneTransitionFactory& operator=(const SceneTransitionFactory&) = delete;

};

