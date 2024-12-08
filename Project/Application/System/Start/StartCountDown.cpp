#include "StartCountDown.h"
#include "../../../Engine/base/Texture/TextureManager.h"
#include "../../../Engine/Math/DeltaTime.h"

void StartCountDown::Initialize()
{

	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// スプライトの位置、色
	const Vector2 kSpritePosition = { 640.0f,200.0f };
	const Vector4 kSpriteColor = { 1.0f, 1.0f, 1.0f, 1.0f };

	// 数字テクスチャハンドル
	numTextureHnadle_ = TextureManager::Load("Resources/Sprite/Common/Number.png", dxCommon);

	// 数字スプライト
	numSprite_.reset(Sprite::Create(numTextureHnadle_, kSpritePosition, kSpriteColor));
	// テクスチャサイズ
	const Vector2 kNumSpriteTextureSize = { 64.0f,64.0f };
	numSprite_->SetTextureSize(kNumSpriteTextureSize);
	// 非表示
	numSprite_->SetIsInvisible(true);
	// サイズ
	const Vector2 kNumSpriteSize = { 128.0f,128.0f };
	numSprite_->SetSize(kNumSpriteSize);

	// startテクスチャハンドル
	startTextureHnadle_ = TextureManager::Load("Resources/Sprite/Common/Start.png", dxCommon);

	// startスプライト
	startSprite_.reset(Sprite::Create(startTextureHnadle_, kSpritePosition, kSpriteColor));
	// 非表示
	startSprite_->SetIsInvisible(true);
	// サイズ
	const Vector2 kStartSpriteSize = { 400.0f,128.0f };
	startSprite_->SetSize(kStartSpriteSize);

	// カウントダウン秒数
	const float kCountDownTime = 100.0f;
	countDownTime_ = kCountDownTime;

	// α値
	alpha_ = kSpriteColor.w;

	// カウントダウン終了したか
	countdownEnded_ = true;
	
	// システム終了したか
	systemEnded_ = false;

}

void StartCountDown::Update()
{

	// システムが終了している
	if (systemEnded_) {
		return;
	}

	// 一回目
	if (countDownTime_ == 100.0f) {
		countdownEnded_ = true;
		const float kCountDownTime = 4.0f;
		countDownTime_ = kCountDownTime;
	}
	else {
		countdownEnded_ = false;
	}

	// カウントダウンを進める
	countDownTime_ -= kDeltaTime_;

	// カウントダウン別処理
	// -1.0f
	if (countDownTime_ <= -1.0f) {
		// カウントダウン終了
		countdownEnded_ = true;
		// システム終了
		systemEnded_ = true;
	}
	// 0.0f
	else if (countDownTime_ <= 0.0f) {
		// カウントダウン終了
		countdownEnded_ = true;
		// 数字を非表示
		numSprite_->SetIsInvisible(true);
		// スタートを表示
		startSprite_->SetIsInvisible(false);
	}
	// 1.0f ～ 3.0f
	else if (countDownTime_ <= 3.0f) {
		// 数字を表示
		numSprite_->SetIsInvisible(false);
		// テクスチャの左上
		float kLeft = static_cast<uint32_t>(countDownTime_ + 1.0f) * 64.0f;
		const Vector2 kTextureLeftTop = { kLeft, 0.0f };
		numSprite_->SetTextureLeftTop(kTextureLeftTop);
	}

}

void StartCountDown::Draw()
{

	// システムが終了している
	if (systemEnded_) {
		return;
	}

	// 数字
	numSprite_->Draw();
	// strat
	startSprite_->Draw();

}
