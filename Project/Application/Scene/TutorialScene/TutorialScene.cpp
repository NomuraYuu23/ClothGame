#include "TutorialScene.h"
#include "../../../Engine/base/WinApp.h"
#include "../../../Engine/base/Texture/TextureManager.h"
#include "../../../Engine/2D/ImguiManager.h"
#include "../../../Engine/base/Debug/D3DResourceLeakChecker.h"
#include "../../../Engine/GlobalVariables/GlobalVariables.h"
#include "../../../Engine/Math/DeltaTime.h"
#include "../../../Engine/3D/Model/ModelDraw.h"
#include "../../Object/Manager/TutorialSceneObjectManager.h"
#include "../../Object/Factory/ObjectFactory.h"

#include "../../../Engine/Physics/ClothGPU/ClothGPU.h"

TutorialScene::~TutorialScene()
{
}

/// <summary>
/// 初期化
/// </summary>
void TutorialScene::Initialize() {

	BaseScene::Initialize();

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

	// 布ライト
	ClothManager::GetInstance()->SetLight(
		directionalLight_.get(),
		pointLightManager_.get(),
		spotLightManager_.get());

	// オブジェクトマネージャー
	objectManager_ = std::make_unique<TutorialSceneObjectManager>();
	objectManager_->Initialize(kLevelIndexTutorial, levelDataManager_);

	// 追従カメラ
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();

	// モデル描画
	ModelDraw::PreDrawParameters preDrawParameters;
	preDrawParameters.directionalLight = directionalLight_.get();
	preDrawParameters.fogManager = FogManager::GetInstance();
	preDrawParameters.pointLightManager = pointLightManager_.get();
	preDrawParameters.spotLightManager = spotLightManager_.get();
	preDrawParameters.environmentTextureHandle = TextureManager::Load("Resources/default/rostock_laage_airport_4k.dds", DirectXCommon::GetInstance());
	ModelDraw::SetPreDrawParameters(preDrawParameters);

	BaseScene::InitilaizeCheck();

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

	// デバッグカメラ
	DebugCameraUpdate();

	// エフェクトマネージャー
	effectManager_->Update(camera_);

}

/// <summary>
/// 描画処理
/// </summary>
void TutorialScene::Draw() {

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

	ModelDraw::PreDraw(dxCommon_->GetCommadList());

	//3Dオブジェクトはここ

	// オブジェクトマネージャー
	objectManager_->Draw(camera_, drawLine_);

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

}
