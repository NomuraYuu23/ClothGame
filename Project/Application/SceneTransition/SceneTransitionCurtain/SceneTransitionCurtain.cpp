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

}

void SceneTransitionCurtain::Update()
{

	ISceneTransition::Update();

	// カメラ
	camera_.Update();

	// カーテン
	curtain_->Update();

}

void SceneTransitionCurtain::Draw()
{

	// カーテン
	curtain_->Draw(camera_);

}

