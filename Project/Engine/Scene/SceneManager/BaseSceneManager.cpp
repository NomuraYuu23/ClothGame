#include "BaseSceneManager.h"

BaseSceneManager::~BaseSceneManager()
{

	if (!sceneDetachCompletion_) {
		sceneInitialize_.join();
	}
	if (!sceneTransitionDetachCompletion_) {
		sceneTransitionInitialize_.join();
	}

}

void BaseSceneManager::Initialize(uint32_t earlySceneNo)
{

	// デタッチ完了フラグ
	sceneTransitionDetachCompletion_ = true;

	// 初期化終了フラグ
	sceneInitializeEnd_ = false;
	sceneTransitionInitializeEnd_ = false;

	// シーン遷移を保持するメンバ変数
	sceneTransition_.reset(sceneTransitionFactory_->CreateSceneTransition(currentSceneNo_, requestSeneNo_));
	sceneTransition_->Initialize();
	sceneTransition_->SetStoppingUpdates(true);
	sceneTransition_->SetIsFadeIn(false);

	// シーンファクトリー
	levelDataManager_ = std::make_unique<LevelDataManager>();
	levelDataManager_->Initialize();
	BaseScene::StaticInitialize(levelDataManager_.get());

	// シーン(タイトル)
	scene_.reset(sceneFactory_->CreateScene(earlySceneNo));
	// シーンの初期化
	sceneInitialize_ = std::thread(std::bind(&BaseSceneManager::SceneInitializeThread, this));
	// デタッチ完了フラグ
	sceneDetachCompletion_ = false;

	// 初期シーン
	currentSceneNo_ = earlySceneNo;

	requestSeneNo_ = earlySceneNo; // リクエストシーン
	prevRequestSeneNo_ = earlySceneNo; // 前のリクエストシーン

}

void BaseSceneManager::Update()
{

	// シーンのチェック
	if (sceneDetachCompletion_) {
		currentSceneNo_ = scene_->GetSceneNo();

		prevRequestSeneNo_ = requestSeneNo_; // 前のリクエストシーン
		requestSeneNo_ = scene_->GetRequestSceneNo(); // リクエストシーン
	}

	// リクエストシーンが変わったか
	if (((requestSeneNo_ != prevRequestSeneNo_) || scene_->GetResetScene())
		&& sceneTransitionDetachCompletion_ && sceneDetachCompletion_) {
		//シーン遷移開始（初期化）
		sceneTransition_.reset(sceneTransitionFactory_->CreateSceneTransition(currentSceneNo_, requestSeneNo_));
		sceneTransitionInitialize_ = std::thread(std::bind(&BaseSceneManager::SceneTransitionInitializeThread, this));
		sceneTransitionDetachCompletion_ = false;
	}

	//シーン遷移中
	if (sceneTransition_ && sceneTransitionDetachCompletion_) {
		// シーン遷移更新
		sceneTransition_->Update();
		if (sceneTransition_->GetSwitchScene() && sceneDetachCompletion_) {
			// シーン切り替え
			currentSceneNo_ = requestSeneNo_;
			scene_->SetStopAudio(true);
			scene_.reset(sceneFactory_->CreateScene(currentSceneNo_));
			// シーンの初期化
			sceneInitialize_ = std::thread(std::bind(&BaseSceneManager::SceneInitializeThread, this));
			sceneDetachCompletion_ = false;
			sceneTransition_->SetSwitchScene(false);
			sceneTransition_->SetStoppingUpdates(true);
		}
		else if (!sceneTransition_->GetTransitioning()) {
			sceneTransition_.reset(nullptr);
		}
	}

	// 更新処理
	if (sceneDetachCompletion_) {
		scene_->Update();
	}

	// シーン初期化終了
	if (sceneInitializeEnd_) {
		sceneInitializeEnd_ = false;
		sceneInitialize_.detach();
		sceneDetachCompletion_ = true;
		if (sceneTransition_) {
			sceneTransition_->SetStoppingUpdates(false);
		}
	}

	// シーン遷移初期化終了
	if (sceneTransitionInitializeEnd_) {
		sceneTransitionInitializeEnd_ = false;
		sceneTransitionInitialize_.detach();
		sceneTransitionDetachCompletion_ = true;
	}

	// 仮レベルデータ表示
	levelDataManager_->ImGuiDraw();

}

void BaseSceneManager::Draw()
{

	// 描画処理
	if (sceneDetachCompletion_) {
		scene_->Draw();
	}
	if (sceneTransition_ && sceneTransitionDetachCompletion_) {
		sceneTransition_->Draw();
	}

}

void BaseSceneManager::SceneInitializeThread()
{

	scene_->Initialize();
	sceneInitializeEnd_ = true;

}

void BaseSceneManager::SceneTransitionInitializeThread()
{

	sceneTransition_->Initialize();
	sceneTransitionInitializeEnd_ = true;

}
