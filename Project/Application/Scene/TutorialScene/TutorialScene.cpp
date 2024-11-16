#include "TutorialScene.h"
#include "../../../Engine/base/WinApp.h"
#include "../../../Engine/base/Texture/TextureManager.h"
#include "../../../Engine/2D/ImguiManager.h"
#include "../../../Engine/base/Debug/D3DResourceLeakChecker.h"
#include "../../../Engine/GlobalVariables/GlobalVariables.h"
#include "../../../Engine/Math/DeltaTime.h"
#include "../../../Engine/3D/Model/ModelDraw.h"
#include "../../Object/Manager/TutorialSceneObjectManager.h"
#include "../../Object/ObjectFactory.h"

#include "../../../Engine/Physics/ClothGPU/ClothGPU.h"

TutorialScene::~TutorialScene()
{
}

/// <summary>
/// 初期化
/// </summary>
void TutorialScene::Initialize() {

	IScene::Initialize();

	ModelCreate();
	TextureLoad();

	// ビュープロジェクション
	EulerTransform baseCameraTransform = {
		1.0f, 1.0f, 1.0f,
		0.58f,0.0f,0.0f,
		0.0f, 23.0f, -35.0f };
	camera_.SetTransform(baseCameraTransform);

	// エフェクトマネージャー
	effectManager_ = EffectManager::GetInstance();
	effectManager_->Initialize(particleCircleModel_.get());

	isDebugCameraActive_ = false;

	collisionManager_.reset(new CollisionManager);
	collisionManager_->Initialize();

	ClothGPU::StaticInitialize(
		dxCommon_->GetDevice(),
		directionalLight_.get(),
		pointLightManager_.get(),
		spotLightManager_.get(),
		FogManager::GetInstance());

	// オブジェクトマネージャー
	objectManager_ = std::make_unique<TutorialSceneObjectManager>();
	ObjectFactory::GetInstance()->Initialize(objectManager_.get());
	objectManager_->Initialize(kLevelIndexTutorial, levelDataManager_);

	// 追従カメラ
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();
	//followCamera_->SetTarget(player_->GetWorldTransformAdress());

	IScene::InitilaizeCheck();

}

/// <summary>
/// 更新処理
/// </summary>
void TutorialScene::Update() {

#ifdef _DEMO
	ImguiDraw();
#endif

	if (requestSceneNo_ == kClear || requestSceneNo_ == kTitle || isBeingReset_) {
		resetScene_ = false;
		return;
	}

	// ゲームへ
	if (input_->TriggerJoystick(JoystickButton::kJoystickButtonY)) {
		requestSceneNo_ = kGame;
	}

	//光源
	directionalLight_->Update(directionalLightData_);
	pointLightManager_->Update(pointLightDatas_);
	spotLightManager_->Update(spotLightDatas_);

	// 追従カメラ
	followCamera_->Update();

	camera_ = static_cast<BaseCamera>(*followCamera_.get());

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

}

/// <summary>
/// 描画処理
/// </summary>
void TutorialScene::Draw() {


	// ポストエフェクト設定
	PostEffect::GetInstance()->SetKernelSize(33);
	PostEffect::GetInstance()->SetGaussianSigma(33.0f);
	PostEffect::GetInstance()->SetProjectionInverse(Matrix4x4::Inverse(camera_.GetProjectionMatrix()));
	PostEffect::GetInstance()->SetRadialBlurStrength(0.2f);
	PostEffect::GetInstance()->SetThreshold(0.25f);

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

	// オブジェクトマネージャー
	objectManager_->Draw(camera_, drawLine_);

	// アウトライン
	PostEffect::GetInstance()->Execution(
		dxCommon_->GetCommadList(),
		renderTargetTexture_,
		PostEffect::kCommandIndexOutline
	);
	WindowSprite::GetInstance()->DrawSRV(PostEffect::GetInstance()->GetEditTextures(0));
	// ブルーム
	PostEffect::GetInstance()->Execution(
		dxCommon_->GetCommadList(),
		renderTargetTexture_,
		PostEffect::kCommandIndexBloom
	);
	WindowSprite::GetInstance()->DrawSRV(PostEffect::GetInstance()->GetEditTextures(0));
	renderTargetTexture_->ClearDepthBuffer();

	// エフェクトマネージャー
	effectManager_->Draw(camera_);

	ModelDraw::PostDraw();

#pragma endregion

#pragma region 線描画

	drawLine_->Draw(dxCommon_->GetCommadList(), camera_);

#pragma endregion


#pragma region パーティクルとエフェクト描画

	// パーティクル描画
	objectManager_->ParticleDraw(camera_);

#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(dxCommon_->GetCommadList());


	//背景
	//前景スプライト描画

	// 前景スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion

}

void TutorialScene::ImguiDraw() {

	ImGui::Begin("Framerate");
	ImGui::Text("Frame rate: %6.2f fps", ImGui::GetIO().Framerate);
	ImGui::End();

	debugCamera_->ImGuiDraw();

	// オブジェクトマネージャー
	objectManager_->ImGuiDraw();

}

void TutorialScene::DebugCameraUpdate()
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

void TutorialScene::ModelCreate()
{
	// パーティクル
	particleCircleModel_.reset(Model::Create("Resources/Particle/", "plane.obj", dxCommon_));

}

void TutorialScene::TextureLoad()
{

	skyboxTextureHandle_ = TextureManager::Load("Resources/default/rostock_laage_airport_4k.dds", DirectXCommon::GetInstance());

}
