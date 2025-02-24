#include "SceneTransitionCurtain.h"
#include "../../../Engine/base/Texture/TextureManager.h"

SceneTransitionCurtain::~SceneTransitionCurtain()
{
}

void SceneTransitionCurtain::Initialize()
{

	BaseSceneTransition::Initialize();

	// カメラ
	camera_.Initialize();

	// 布
	ClothManager::GetInstance()->SetLight(
		directionalLight_.get(),
		pointLightManager_.get(),
		spotLightManager_.get());

	// カーテン
	curtain_ = std::make_unique<Curtain>();
	curtain_->Initialize();

	// テクスチャ読み込み
	blockTextureHandle_ = TextureManager::Load("Resources/default/white2x2.png", DirectXCommon::GetInstance());
	blockColor_ = { 0.0f,0.0f,0.0f,0.0f };
	// 黒位置
	const Vector2 kBlockSpritePosition = { static_cast<float>(WinApp::kWindowWidth_) / 2.0f, static_cast<float>(WinApp::kWindowHeight_) / 2.0f };
	blockSprite_.reset(Sprite::Create(blockTextureHandle_, kBlockSpritePosition, blockColor_));
	// 黒大きさ
	const Vector2 kBlockSpriteSize = { static_cast<float>(WinApp::kWindowWidth_), static_cast<float>(WinApp::kWindowHeight_) };
	blockSprite_->SetSize(kBlockSpriteSize);

	fadeInTime_ = 3.0f; //フェードインタイム
	fadeOutTime_ = 3.0f; //フェードアウトタイム

}

void SceneTransitionCurtain::Update()
{

	BaseSceneTransition::Update();

	// カメラ
	camera_.Update();

	// カーテン
	// α、t値変更
	if (isFadeIn_) {
		curtain_->Update(fadeTimer_ / fadeInTime_);
		blockColor_.w = fadeTimer_ / fadeInTime_;
	}
	else {
		curtain_->Update(1.0f - fadeTimer_ / fadeOutTime_);
		blockColor_.w = 1.0f - fadeTimer_ / fadeOutTime_;
	}

	blockSprite_->SetColor(blockColor_);

}

void SceneTransitionCurtain::Draw()
{

	DirectXCommon* directXCommon = DirectXCommon::GetInstance();

	// 前景スプライト描画前処理
	Sprite::PreDraw(directXCommon->GetCommadList());

	// 黒背景
	blockSprite_->Draw();

	// 前景スプライト描画後処理
	Sprite::PostDraw();

	// 深度バッファクリア
	directXCommon->GetRenderTargetTexture()->ClearDepthBuffer();

	// カーテン
	curtain_->Draw(camera_);

}

