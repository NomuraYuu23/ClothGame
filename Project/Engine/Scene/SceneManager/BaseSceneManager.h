#pragma once
#include "../IScene/IScene.h"
#include <thread>
#include "../../Level/LevelDataManager.h"
#include "../SceneFactory/AbstractSceneFactory.h"
#include "../../SceneTransition/ISceneTransition/ISceneTransition.h"
#include "../../SceneTransition/SceneTransitionFactory/AbstractSceneTransitionFactory.h"

/// <summary>
/// シーンマネージャー基盤クラス
/// </summary>
class BaseSceneManager
{

public: // メンバ関数

	/// <summary>
	/// デストラクタ
	/// </summary>
	~BaseSceneManager();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="earlySceneNo">最初のシーン</param>
	virtual void Initialize(uint32_t earlySceneNo);

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

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

protected: // メンバ変数

	// シーンファクトリー	
	AbstractSceneFactory* sceneFactory_ = nullptr;

	// シーンを保持するメンバ変数
	std::unique_ptr<IScene> scene_;

	// どのステージを呼び出すかを管理する変数
	int currentSceneNo_; // 現在のシーン

	int requestSeneNo_; // リクエストシーン
	int prevRequestSeneNo_; // 前のリクエストシーン

	// シーン遷移ファクトリー
	AbstractSceneTransitionFactory* sceneTransitionFactory_ = nullptr;

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

