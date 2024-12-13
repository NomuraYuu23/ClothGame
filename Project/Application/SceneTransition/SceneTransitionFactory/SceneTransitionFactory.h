#pragma once
#include "../../../Engine/SceneTransition/SceneTransitionFactory/AbstractSceneTransitionFactory.h"

/// <summary>
/// シーン遷移用ファクトリー
/// </summary>
class SceneTransitionFactory : 
	public AbstractSceneTransitionFactory{

public: // 静的メンバ関数

	/// <summary>
	/// インスタンス取得
	/// </summary>
	/// <returns></returns>
	static SceneTransitionFactory* GetInstance();

public: // メンバ関数

	/// <summary>
	/// シーン遷移生成
	/// </summary>
	/// <param name="sceneName">シーンの名前</param>
	/// <param name="requestSeneName">リクエストシーンの名前</param>
	/// <returns></returns>
	ISceneTransition* CreateSceneTransition(int sceneName, int requestSeneName) override;

private: // シングルトン
	SceneTransitionFactory() = default;
	~SceneTransitionFactory() = default;
	SceneTransitionFactory(const SceneTransitionFactory&) = delete;
	const SceneTransitionFactory& operator=(const SceneTransitionFactory&) = delete;

};

