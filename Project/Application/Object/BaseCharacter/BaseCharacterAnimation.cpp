#include "BaseCharacterAnimation.h"

void BaseCharacterAnimation::Initialize(Model* model)
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
	currentMotionNo_ = 0;

	// 前のモーション番号
	prevMotionNo_ = 0;

	// 待ちアニメーション
	animation_.StartAnimation(0, true);

}

void BaseCharacterAnimation::Update(uint32_t stateMotionNo)
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
