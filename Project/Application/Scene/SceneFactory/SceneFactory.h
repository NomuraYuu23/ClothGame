#pragma once
#include "../../../Engine/Scene/SceneFactory/AbstractSceneFactory.h"

/// <summary>
/// シーンファクトリー
/// </summary>
class SceneFactory : public AbstractSceneFactory{

public: // 静的メンバ関数

	/// <summary>
	/// インスタンス取得
	/// </summary>
	/// <returns></returns>
	static SceneFactory* GetInstance();

public: // メンバ関数

	// シーン生成
	IScene* CreateScene(int sceneName) override;

private: 
	SceneFactory() = default;
	~SceneFactory() = default;
	SceneFactory(const SceneFactory&) = delete;
	const SceneFactory& operator=(const SceneFactory&) = delete;

};

