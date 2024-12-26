#include "TitleScene.h"
#include "../../../Engine/base/Texture/TextureManager.h"
#include "../../../Engine/2D/ImguiManager.h"
#include "../../../Engine/GlobalVariables/GlobalVariables.h"
#include "../../../Engine/Math/Ease.h"
#include "../../../Engine/3D/Model/ModelDraw.h"
#include "../../Object/Manager/TitleSceneObjectManager.h"
#include "../../Object/Factory/ObjectFactory.h"

TitleScene::~TitleScene()
{

}

void TitleScene::Initialize()
{

	IScene::Initialize();

	ModelCreate();
	TextureLoad();

	// オブジェクトマネージャー
	objectManager_ = std::make_unique<TitleSceneObjectManager>();
	objectManager_->Initialize(kLevelIndexTitle, levelDataManager_);

	const EulerTransform cameraTransform = {
		1.0f,1.0f,1.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, -10.0f};

	camera_.SetTransform(cameraTransform);
	camera_.Update();

	// ボタンスプライト位置
	const Vector2 kButtonSpritePosition = { 400.0f, 540.0f };
	buttonColor_ = { 1.0f, 1.0f, 1.0f, 1.0f };
	buttonSprite_.reset(Sprite::Create(buttonTextureHandle_, kButtonSpritePosition, buttonColor_));
	buttonAlphaT_ = 0.0f;
	// α値変更速度
	const float kButtonAlphaTSpeed = 0.01f;
	buttonAlphaTSpeed_ = kButtonAlphaTSpeed;
	buttonItIncreaseAlphaT_ = true;

	// クリアスプライト位置
	const Vector2 kTitleSpritePosition = { 640.0f, 360.0f };
	titleSprite_.reset(Sprite::Create(titleTextureHandle_, kTitleSpritePosition, { 1.0f, 1.0f, 1.0f, 1.0f }));

	// タイトル背景
	titleBackGround_ = std::make_unique<TitleBackGround>();
	titleBackGround_->Initialize();

	IScene::InitilaizeCheck();

}

void TitleScene::Update()
{

	if (input_->TriggerJoystick(JoystickButton::kJoystickButtonA)) {
		// 行きたいシーンへ
		requestSceneNo_ = kGame;
	}

	objectManager_->Update();

	// デバッグカメラ
	DebugCameraUpdate();

	// ボタンスプライト
	if (buttonItIncreaseAlphaT_) {
		buttonAlphaT_ += buttonAlphaTSpeed_;
		if (buttonAlphaT_ >= 1.0f) {
			buttonAlphaT_ = 1.0f;
			buttonItIncreaseAlphaT_ = false;
		}
	}
	else {
		buttonAlphaT_ -= buttonAlphaTSpeed_;
		if (buttonAlphaT_ <= 0.0f) {
			buttonAlphaT_ = 0.0f;
			buttonItIncreaseAlphaT_ = true;
		}
	}
	buttonColor_.w = Ease::Easing(Ease::EaseName::Lerp, 0.0f, 1.0f, buttonAlphaT_);
	buttonSprite_->SetColor(buttonColor_);

	// ボタンスプライト
	if (buttonItIncreaseAlphaT_) {
		buttonAlphaT_ += buttonAlphaTSpeed_;
		if (buttonAlphaT_ >= 1.0f) {
			buttonAlphaT_ = 1.0f;
			buttonItIncreaseAlphaT_ = false;
		}
	}
	else {
		buttonAlphaT_ -= buttonAlphaTSpeed_;
		if (buttonAlphaT_ <= 0.0f) {
			buttonAlphaT_ = 0.0f;
			buttonItIncreaseAlphaT_ = true;
		}
	}
	buttonColor_.w = Ease::Easing(Ease::EaseName::Lerp, 0.0f, 1.0f, buttonAlphaT_);
	buttonSprite_->SetColor(buttonColor_);

	// タイトル背景
	titleBackGround_->Update();

	ImguiDraw();

}

void TitleScene::Draw()
{

	// スプライト描画前処理
	Sprite::PreDraw(dxCommon_->GetCommadList());

	//背景
	//前景スプライト描画
	//titleSprite_->Draw();

	//buttonSprite_->Draw();

	// 前景スプライト描画後処理
	Sprite::PostDraw();

	// 深度バッファクリア
	renderTargetTexture_->ClearDepthBuffer();

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

	// タイトル背景
	titleBackGround_->Draw(camera_);

	ModelDraw::PostDraw();

#pragma endregion

	// パーティクル描画
	objectManager_->ParticleDraw(camera_);

}

void TitleScene::ImguiDraw()
{

#ifdef _DEMO

	PostEffect::GetInstance()->ImGuiDraw();

	debugCamera_->ImGuiDraw();

#endif // _DEMO

}

void TitleScene::DebugCameraUpdate()
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

void TitleScene::ModelCreate()
{

}

void TitleScene::TextureLoad()
{

	buttonTextureHandle_ = TextureManager::Load("Resources/UI/ButtonA.png", dxCommon_);

	titleTextureHandle_ = TextureManager::Load("Resources/OutGame/Title.png", dxCommon_);

	skyboxTextureHandle_ = TextureManager::Load("Resources/default/rostock_laage_airport_4k.dds", DirectXCommon::GetInstance());

}
