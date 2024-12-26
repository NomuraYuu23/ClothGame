#include "ClearScene.h"
#include "../../../Engine/base/Texture/TextureManager.h"
#include "../../../Engine/2D/ImguiManager.h"
#include "../../../Engine/GlobalVariables/GlobalVariables.h"
#include "../../../Engine/Math/Ease.h"

ClearScene::~ClearScene()
{

}

void ClearScene::Initialize()
{

	IScene::Initialize();

	ModelCreate();
	TextureLoad();

	const EulerTransform cameraTransform = {
	1.0f,1.0f,1.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, -10.0f };

	camera_.SetTransform(cameraTransform);
	camera_.Update();

	// ボタンスプライト位置
	const Vector2 kButtonSpritePosition = { 640.0f, 560.0f };
	buttonColor_ = { 1.0f, 1.0f, 1.0f, 1.0f };
	buttonSprite_.reset(Sprite::Create(buttonTextureHandle_, kButtonSpritePosition, buttonColor_));
	buttonAlphaT_ = 0.0f;
	// α値変更速度
	const float kButtonAlphaTSpeed = 0.01f;
	buttonAlphaTSpeed_ = kButtonAlphaTSpeed;
	buttonItIncreaseAlphaT_ = true;

	// 背景
	clearBackGround_ = std::make_unique<ClearBackGround>();
	clearBackGround_->Initialize();

	// クリアスプライト位置
	const Vector2 kClearSpritePosition = { 640.0f, 360.0f };
	// クリアスプライト色
	const Vector4 kClearSpriteColor = { 0.6f, 1.0f, 0.6f, 1.0f };
	clearSprite_.reset(Sprite::Create(clearTextureHandle_, kClearSpritePosition, kClearSpriteColor));

}

void ClearScene::Update()
{

	if (input_->TriggerJoystick(JoystickButton::kJoystickButtonA)) {
		// 行きたいシーンへ
		requestSceneNo_ = kTitle;
	}


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

	clearBackGround_->Update();
	
}

void ClearScene::Draw()
{

#pragma region モデル描画

	ModelDraw::PreDrawDesc preDrawDesc;
	preDrawDesc.commandList = dxCommon_->GetCommadList();
	preDrawDesc.directionalLight = directionalLight_.get();
	preDrawDesc.fogManager = FogManager::GetInstance();
	preDrawDesc.pointLightManager = pointLightManager_.get();
	preDrawDesc.spotLightManager = spotLightManager_.get();
	preDrawDesc.environmentTextureHandle = skyboxTextureHandle_;
	ModelDraw::PreDraw(preDrawDesc);

	// タイトル背景
	clearBackGround_->Draw(camera_);

	ModelDraw::PostDraw();

#pragma endregion

#pragma region スプライト描画
	// スプライト描画前処理
	Sprite::PreDraw(dxCommon_->GetCommadList());

	//背景
	//前景スプライト描画
	clearSprite_->Draw();

	buttonSprite_->Draw();

	// 前景スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion

}

void ClearScene::ImguiDraw()
{

#ifdef _DEBUG
#endif // _DEBUG

}

void ClearScene::ModelCreate()
{

}

void ClearScene::TextureLoad()
{

	buttonTextureHandle_ = TextureManager::Load("Resources/UI/ButtonA.png", dxCommon_);

	clearTextureHandle_ = TextureManager::Load("Resources/OutGame/Clear.png", dxCommon_);

	skyboxTextureHandle_ = TextureManager::Load("Resources/default/rostock_laage_airport_4k.dds", DirectXCommon::GetInstance());

}
