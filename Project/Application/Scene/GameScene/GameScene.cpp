#include "GameScene.h"
#include "../../../Engine/base/WinApp.h"
#include "../../../Engine/base/Texture/TextureManager.h"
#include "../../../Engine/2D/ImguiManager.h"
#include "../../../Engine/base/Debug/D3DResourceLeakChecker.h"
#include "../../../Engine/GlobalVariables/GlobalVariables.h"
#include "../../../Engine/Math/DeltaTime.h"
#include "../../../Engine/3D/Model/ModelDraw.h"
#include "../../../Engine/base/WindowSprite/WindowSprite.h"
#include "../../Object/Manager/GameSceneObjectManager.h"
#include "../../Object/Factory/ObjectFactory.h"
#include "../../../Engine/Physics/ClothGPU/ClothGPU.h"

GameScene::~GameScene()
{
}

/// <summary>
/// 初期化
/// </summary>
void GameScene::Initialize() {

	IScene::Initialize();

	ModelCreate();
	TextureLoad();

	// ビュープロジェクション
	const EulerTransform baseCameraTransform = {
		1.0f, 1.0f, 1.0f,
		0.58f,0.0f,0.0f,
		0.0f, 23.0f, -35.0f };
	camera_.SetTransform(baseCameraTransform);

	// エフェクトマネージャー
	effectManager_ = EffectManager::GetInstance();
	effectManager_->Initialize(particleCircleModel_.get());

	isDebugCameraActive_ = false;

	collisionManager_.reset(new GameSceneCollisionManager);
	collisionManager_->Initialize();

	ClothGPU::StaticInitialize(
		dxCommon_->GetDevice(),
		directionalLight_.get(),
		pointLightManager_.get(),
		spotLightManager_.get(),
		FogManager::GetInstance());

	// オブジェクトマネージャー
	objectManager_ = std::make_unique<GameSceneObjectManager>();
	objectManager_->Initialize(kLevelIndexMain, levelDataManager_);

	// 追従カメラ
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();
	followCamera_->SetTarget(objectManager_->GetObjectPointer("Player")->GetWorldTransformAdress());

	// UIマネージャー
	uiManager_ = std::make_unique<UIManager>();
	uiManager_->Initialize();

	// スタートカウントダウン
	startCountDown_ = std::make_unique<StartCountDown>();
	startCountDown_->Initialize();

	// ポストエフェクト
	postEffectSystem_ = std::make_unique<PostEffectSystem>();
	postEffectSystem_->Initialize();
	postEffectSystem_->SetPlayer(static_cast<Player*>(objectManager_->GetObjectPointer("Player")));
	postEffectSystem_->SetRenderTargetTexture(renderTargetTexture_);

	IScene::InitilaizeCheck();

}

/// <summary>
/// 更新処理
/// </summary>
void GameScene::Update() {

#ifdef _DEMO
	ImguiDraw();
#endif

	if (requestSceneNo_ == kClear || requestSceneNo_ == kTitle || isBeingReset_) {
		resetScene_ = false;
		return;
	}

	//光源
	directionalLight_->Update(directionalLightData_);
	pointLightManager_->Update(pointLightDatas_);
	spotLightManager_->Update(spotLightDatas_);

	// 追従カメラ
	followCamera_->Update();

	camera_ = static_cast<BaseCamera>(*followCamera_.get());

	// スタートカウントダウン
	startCountDown_->Update();
	if (!startCountDown_->GetCountdownEnded()) {
		return;
	}

	// オブジェクトマネージャー
	objectManager_->Update();

	// あたり判定
	collisionManager_->ListClear();

	objectManager_->CollisionListRegister(collisionManager_.get());

	collisionManager_->CheakAllCollision();

	// デバッグカメラ
	DebugCameraUpdate();
	// エフェクトマネージャー
	effectManager_->Update(camera_);
	// UIマネージャー
	uiManager_->Update();
	// ポストエフェクト
	postEffectSystem_->Update();

	// ゲームが終了するか
	if (static_cast<GameSceneObjectManager*>(objectManager_.get())->GetLevelChangeEnd()) {
		requestSceneNo_ = kClear;
	}

}

/// <summary>
/// 描画処理
/// </summary>
void GameScene::Draw() {

	//ゲームの処理 

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(dxCommon_->GetCommadList());

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	renderTargetTexture_->ClearDepthBuffer();


#pragma endregion

#pragma region モデル描画

	ModelDraw::PreDrawDesc preDrawDesc;
	preDrawDesc.commandList = dxCommon_->GetCommadList();
	preDrawDesc.directionalLight = directionalLight_.get();
	preDrawDesc.fogManager = FogManager::GetInstance();
	preDrawDesc.pointLightManager = pointLightManager_.get();
	preDrawDesc.spotLightManager = spotLightManager_.get();
	preDrawDesc.environmentTextureHandle = skyboxTextureHandle_;
	ModelDraw::PreDraw(preDrawDesc);

	//3Dオブジェクトはここ

	objectManager_->Draw(camera_, drawLine_);

	// エフェクトマネージャー
	effectManager_->Draw(camera_);

	ModelDraw::PostDraw();

#pragma endregion

#pragma region 線描画

	drawLine_->Draw(dxCommon_->GetCommadList(), camera_);

#pragma endregion

#pragma region パーティクル描画

	// パーティクル描画
	objectManager_->ParticleDraw(camera_);

#pragma endregion

	// ポストエフェクト
	postEffectSystem_->Execute();

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(dxCommon_->GetCommadList());
	

	//背景
	//前景スプライト描画

	// UI
	uiManager_->Draw();

	// システム
	startCountDown_->Draw();

	// 前景スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion

}

void GameScene::ImguiDraw(){

	ImGui::Begin("Framerate");
	ImGui::Text("Frame rate: %6.2f fps", ImGui::GetIO().Framerate);
	ImGui::End();

	debugCamera_->ImGuiDraw();

	objectManager_->ImGuiDraw();

	PostEffect::GetInstance()->ImGuiDraw();

	uiManager_->ImGuiDraw();

}

void GameScene::DebugCameraUpdate()
{

#ifdef _DEMO
	if (input_->TriggerKey(DIK_RETURN)) {
		if (isDebugCameraActive_) {
			isDebugCameraActive_ = false;
		}
		else {
			isDebugCameraActive_ = true;
		}
	}

	// カメラの処理
	if (isDebugCameraActive_) {
		// デバッグカメラの更新
		debugCamera_->Update();
		// デバッグカメラのビュー行列をコピー
		camera_ = static_cast<BaseCamera>(*debugCamera_.get());
		// ビュー行列の転送
		camera_.Update();
	}
#endif

}

void GameScene::ModelCreate()
{

	particleCircleModel_.reset(Model::Create("Resources/Particle/", "plane.obj", dxCommon_));

}

void GameScene::TextureLoad()
{

	skyboxTextureHandle_ = TextureManager::Load("Resources/default/rostock_laage_airport_4k.dds", DirectXCommon::GetInstance());

}
