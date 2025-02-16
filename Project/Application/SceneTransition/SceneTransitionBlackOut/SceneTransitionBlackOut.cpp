#include "SceneTransitionBlackOut.h"
#include "../../../Engine/base/Texture/TextureManager.h"
#include "../../../Engine/base/DxCommon/DirectXCommon.h"
#include "../../../Engine/base/WinApp.h"

SceneTransitionBlackOut::~SceneTransitionBlackOut()
{

}

void SceneTransitionBlackOut::Initialize()
{

	BaseSceneTransition::Initialize();

	// テクスチャ読み込み
	blockTextureHandle_ = TextureManager::Load("Resources/default/white2x2.png", DirectXCommon::GetInstance());
	blockColor_ = { 0.0f,0.0f,0.0f,0.0f };
	// 黒位置
	const Vector2 kBlockSpritePosition = { static_cast<float>(WinApp::kWindowWidth_) / 2.0f, static_cast<float>(WinApp::kWindowHeight_) / 2.0f };
	blockSprite_.reset(Sprite::Create(blockTextureHandle_, kBlockSpritePosition, blockColor_));
	// 黒大きさ
	const Vector2 kBlockSpriteSize = { static_cast<float>(WinApp::kWindowWidth_), static_cast<float>(WinApp::kWindowHeight_) };
	blockSprite_->SetSize(kBlockSpriteSize);

	// テクスチャ読み込み
	loadTextureHandle_ = TextureManager::Load("Resources/default/load.png", DirectXCommon::GetInstance());
	// ロード位置
	const Vector2 kLoadSpritePosition = { 1100.0f, 600.0f };
	const Vector4 kLoadSpriteColor = { 1.0f,1.0f,1.0f, 0.0f };
	loadSprite_.reset(Sprite::Create(loadTextureHandle_, kLoadSpritePosition, kLoadSpriteColor));
	// ロード大きさ
	const Vector2 kLoadSpriteSize = { 128.0f, 128.0f };
	loadSprite_->SetSize(kLoadSpriteSize);
	loadSprite_->SetTextureSize(kLoadSpriteSize);
	loadSprite_->SetTextureLeftTop({ 0.0f, 0.0f });
	loadCount_ = 0;

}

void SceneTransitionBlackOut::Update()
{

	BaseSceneTransition::Update();

	// α値変更
	if (isFadeIn_) {
		blockColor_.w = fadeTimer_ / fadeInTime_;
	}
	else {
		blockColor_.w = 1.0f - fadeTimer_ / fadeOutTime_;
	}

	// カウント速度
	const int kCountSpeed = 2;
	// カウント最大数
	const int kCountMax = 80;
	// LeftTopで使う値
	const int kCountDivide = kCountMax / 8;
	// ロード大きさX
	const float kLoadSpriteSizeX = 128.0f;

	loadCount_ = loadCount_ + kCountSpeed % kCountMax;
	loadSprite_->SetTextureLeftTop({ kLoadSpriteSizeX * (static_cast<float>(loadCount_ / kCountDivide)), 0.0f });

	blockSprite_->SetColor(blockColor_);
	loadSprite_->SetColor({ 1.0f,1.0f,1.0f,blockColor_.w });

}

void SceneTransitionBlackOut::Draw()
{

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(DirectXCommon::GetInstance()->GetCommadList());

	blockSprite_->Draw();
	loadSprite_->Draw();

	// 前景スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion

}
