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

	buttonSprite_.reset(Sprite::Create(buttonTextureHandle_, { 400.0f, 540.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }));
	buttonAlphaT_ = 0.0f;
	buttonAlphaTSpeed_ = 0.01f;
	buttonItIncreaseAlphaT_ = true;
	buttonColor_ = { 1.0f, 1.0f, 1.0f, 1.0f };

	clearSprite_.reset(Sprite::Create(clearTextureHandle_, { 640.0f, 360.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }));

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
	
}

void ClearScene::Draw()
{

#pragma region 前景スプライト描画
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

}
