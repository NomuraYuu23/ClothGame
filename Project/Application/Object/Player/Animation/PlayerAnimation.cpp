#include "PlayerAnimation.h"
#include "../Player.h"

void PlayerAnimation::Initialize(Model* model)
{

	// ローカル行列マネージャ
	localMatrixManager_ = std::make_unique<LocalMatrixManager>();
	localMatrixManager_->Initialize(model->GetRootNode());

	// アニメーション
	animation_.Initialize(
		model->GetNodeAnimationData(),
		localMatrixManager_->GetInitTransform(),
		localMatrixManager_->GetNodeNames());

	// 現在のモーション番号
	currentMotionNo_ = PlayerMotionIndex::kPlayerMotionIndexWait;

	// 前のモーション番号
	prevMotionNo_ = PlayerMotionIndex::kPlayerMotionIndexWait;

	// 待ちアニメーション
	animation_.StartAnimation(PlayerMotionIndex::kPlayerMotionIndexWait, true);

}

void PlayerAnimation::Update(uint32_t stateMotionNo)
{

	// 番号確認
	prevMotionNo_ = currentMotionNo_;
	currentMotionNo_ = stateMotionNo;

	// 番号切り替え
	if (currentMotionNo_ != prevMotionNo_) {
		animation_.StopAnimation(prevMotionNo_);
		animation_.StartAnimation(currentMotionNo_, true);
	}

	// ローカル行列マネージャ
	localMatrixManager_->SetNodeLocalMatrix(animation_.AnimationUpdate());
	localMatrixManager_->Map();

}
