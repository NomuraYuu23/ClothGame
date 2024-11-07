#include "NodeAnimationData.h"

const double NodeAnimationData::kSecondsConversion = 1000.0;

void NodeAnimationData::Initialize(const aiNodeAnim& nodeAnim)
{

	// NodeAnimationKey
	AnimationVector3Key animationVector3Key;
	AnimationQuaternionKey animationQuaternionKey;

	// 終了時間
	endTime_ = 0.0f;

	// 位置
	positionKeyNum_ = nodeAnim.mNumPositionKeys;

	for (uint32_t i = 0; i < positionKeyNum_; ++i) {
		
		animationVector3Key.value_.x = -nodeAnim.mPositionKeys[i].mValue.x;
		animationVector3Key.value_.y = nodeAnim.mPositionKeys[i].mValue.y;
		animationVector3Key.value_.z = nodeAnim.mPositionKeys[i].mValue.z;

		animationVector3Key.time_ = nodeAnim.mPositionKeys[i].mTime / kSecondsConversion;

		positions_.push_back(animationVector3Key);
		
		if (endTime_ < animationVector3Key.time_) {
			endTime_ = animationVector3Key.time_;
		}

	}

	// 回転
	rotationKeyNum_ = nodeAnim.mNumRotationKeys;

	for (uint32_t i = 0; i < rotationKeyNum_; ++i) {

		animationQuaternionKey.value_.x = nodeAnim.mRotationKeys[i].mValue.x;
		animationQuaternionKey.value_.y = -nodeAnim.mRotationKeys[i].mValue.y;
		animationQuaternionKey.value_.z = -nodeAnim.mRotationKeys[i].mValue.z;
		animationQuaternionKey.value_.w = nodeAnim.mRotationKeys[i].mValue.w;

		animationQuaternionKey.time_ = nodeAnim.mRotationKeys[i].mTime / kSecondsConversion;

		rotations_.push_back(animationQuaternionKey);

		if (endTime_ < animationQuaternionKey.time_) {
			endTime_ = animationQuaternionKey.time_;
		}

	}

	// 大きさ
	scalingKeyNum_ = nodeAnim.mNumScalingKeys;

	for (uint32_t i = 0; i < scalingKeyNum_; ++i) {

		animationVector3Key.value_.x = nodeAnim.mScalingKeys[i].mValue.x;
		animationVector3Key.value_.y = nodeAnim.mScalingKeys[i].mValue.y;
		animationVector3Key.value_.z = nodeAnim.mScalingKeys[i].mValue.z;

		animationVector3Key.time_ = nodeAnim.mScalingKeys[i].mTime / kSecondsConversion;

		scalings_.push_back(animationVector3Key);

		if (endTime_ < animationVector3Key.time_) {
			endTime_ = animationVector3Key.time_;
		}

	}

	nodeName_ = nodeAnim.mNodeName.C_Str();

}
