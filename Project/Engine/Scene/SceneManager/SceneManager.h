#pragma once

#include "../IScene/IScene.h"
#include "../../../Application/Scene/SceneFactory/SceneFactory.h"
#include "../../SceneTransition/ISceneTransition/ISceneTransition.h"
#include "../../../Application/SceneTransition/SceneTransitionFactory/SceneTransitionFactory.h"
#include <thread>
#include "../../Level/LevelDataManager.h"

/// <summary>
/// シーンマネージャー
/// </summary>
class SceneManager
{

public: // メンバ関数


	/// <summary>
	/// デストラクタ
	/// </summary>
	~SceneManager();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(uint32_t earlySceneNo);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 初期化を回す関数
	/// </summary>
	void SceneInitializeThread();

	/// <summary>
	/// 初期化を回す関数
	/// </summary>
	void SceneTransitionInitializeThread();

private: // メンバ変数

	// シーンファクトリー	
	SceneFactory* sceneFacyory_ = nullptr;

	// シーンを保持するメンバ変数
	std::unique_ptr<IScene> scene_;

	// どのステージを呼び出すかを管理する変数
	int currentSceneNo_; // 現在のシーン

	int requestSeneNo_; // リクエストシーン
	int prevRequestSeneNo_; // 前のリクエストシーン

	// シーン遷移ファクトリー
	SceneTransitionFactory* sceneTransitionFactory_ = nullptr;

	// シーン遷移を保持するメンバ変数
	std::unique_ptr<ISceneTransition> sceneTransition_;

	// シーン初期化用スレッド
	std::thread sceneInitialize_;
	// シーン初期化が終わったか
	bool sceneInitializeEnd_;
	// シーン初期化がデタッチされたか
	bool sceneDetachCompletion_;

	// シーン遷移用初期化用スレッド
	std::thread sceneTransitionInitialize_;
	// シーン遷移初期化が終わったか
	bool sceneTransitionInitializeEnd_;
	// シーン遷移初期化が最初か
	bool isFirstLoad_;
	// シーン遷移初期化がデタッチされたか
	bool sceneTransitionDetachCompletion_;

	// レベルデータマネージャー
	std::unique_ptr<LevelDataManager> levelDataManager_;

};

