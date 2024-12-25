#include "SceneTransitionCurtain.h"

SceneTransitionCurtain::~SceneTransitionCurtain()
{
}

void SceneTransitionCurtain::Initialize()
{

	ISceneTransition::Initialize();

	// カメラ
	camera_.Initialize();

	// 布
	ClothGPU::SetLight(
		directionalLight_.get(),
		pointLightManager_.get(),
		spotLightManager_.get());

	// カーテン
	curtain_ = std::make_unique<Curtain>();
	curtain_->Initialize();

	fadeInTime_ = 5.0f; //フェードインタイム
	fadeOutTime_ = 5.0f; //フェードアウトタイム

}

void SceneTransitionCurtain::Update()
{

	ISceneTransition::Update();

	// カメラ
	camera_.Update();

	// カーテン
	// t値変更
	if (isFadeIn_) {
		curtain_->Update(fadeTimer_ / fadeInTime_);
	}
	else {
		curtain_->Update(1.0f - fadeTimer_ / fadeOutTime_);
	}

}

void SceneTransitionCurtain::Draw()
{

	// カーテン
	curtain_->Draw(camera_);

}

