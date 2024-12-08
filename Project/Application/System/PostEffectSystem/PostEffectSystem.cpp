#include "PostEffectSystem.h"
#include "../../Object/Player/Player.h"

void PostEffectSystem::Initialize()
{

	// ポストエフェクト
	postEffect_ = PostEffect::GetInstance();
	// DirectXCommon
	dxCommon_ = DirectXCommon::GetInstance();
	// ウインドウスプライト
	windowSprite_ = WindowSprite::GetInstance();
	
	// ラジアルブラー実行するか
	runRadialBlur_ = false;

}

void PostEffectSystem::Update()
{

	// ラジアルブラー
	postEffect_->SetRadialBlurStrength(0.2f);
	runRadialBlur_ = player_->GetCurrentStateNo() == kPlayerStateIndexDash;

}

void PostEffectSystem::Execute()
{

	// ラジアルブラー
	if (runRadialBlur_) {
		postEffect_->Execution(
			dxCommon_->GetCommadList(),
			renderTargetTexture_,
			PostEffect::kCommandIndexRadialBlur
		);
		windowSprite_->DrawSRV(PostEffect::GetInstance()->GetEditTextures(0));
	}

}
