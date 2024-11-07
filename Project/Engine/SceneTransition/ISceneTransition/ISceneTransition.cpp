#include "ISceneTransition.h"
#include "../../Math/DeltaTime.h"

ISceneTransition::~ISceneTransition()
{

}

void ISceneTransition::Initialize()
{

	fadeInTime_ = 1.0f; //フェードインタイム
	fadeOutTime_ = 1.0f; //フェードアウトタイム
	fadeTimer_ = 0.0f; // タイマー

	transitioning_ = true; //遷移しているか
	isFadeIn_ = true; // フェードインか
	switchScene_ = false; //シーンを切り替える

	stoppingUpdates_ = false; // 更新を停止する

}

void ISceneTransition::Update()
{

	if (stoppingUpdates_) {
		return;
	}

	fadeTimer_ += kDeltaTime_;

	if (isFadeIn_) {
		if (fadeTimer_ >= fadeInTime_) {
			fadeTimer_ = 0.0f;
			isFadeIn_ = false;
			switchScene_ = true;
		}
	}
	else {
		if (fadeTimer_ >= fadeOutTime_) {
			transitioning_ = false;
		}
	}

}
