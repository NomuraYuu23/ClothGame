#include "NodeAnimationData.h"

const double NodeAnimationData::kSecondsConversion_ = 1000.0;

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
		
		animationVector3Key.value.x = -nodeAnim.mPositionKeys[i].mValue.x;
		animationVector3Key.value.y = nodeAnim.mPositionKeys[i].mValue.y;
		animationVector3Key.value.z = nodeAnim.mPositionKeys[i].mValue.z;

		animationVector3Key.time = nodeAnim.mPositionKeys[i].mTime / kSecondsConversion_;

		positions_.push_back(animationVector3Key);
		
		if (endTime_ < animationVector3Key.time) {
			endTime_ = animationVector3Key.time;
		}

	}

	// 回転
	rotationKeyNum_ = nodeAnim.mNumRotationKeys;

	for (uint32_t i = 0; i < rotationKeyNum_; ++i) {

		animationQuaternionKey.value.x = nodeAnim.mRotationKeys[i].mValue.x;
		animationQuaternionKey.value.y = -nodeAnim.mRotationKeys[i].mValue.y;
		animationQuaternionKey.value.z = -nodeAnim.mRotationKeys[i].mValue.z;
		animationQuaternionKey.value.w = nodeAnim.mRotationKeys[i].mValue.w;

		animationQuaternionKey.time = nodeAnim.mRotationKeys[i].mTime / kSecondsConversion_;

		rotations_.push_back(animationQuaternionKey);

		if (endTime_ < animationQuaternionKey.time) {
			endTime_ = animationQuaternionKey.time;
		}

	}

	// 大きさ
	scalingKeyNum_ = nodeAnim.mNumScalingKeys;

	for (uint32_t i = 0; i < scalingKeyNum_; ++i) {

		animationVector3Key.value.x = nodeAnim.mScalingKeys[i].mValue.x;
		animationVector3Key.value.y = nodeAnim.mScalingKeys[i].mValue.y;
		animationVector3Key.value.z = nodeAnim.mScalingKeys[i].mValue.z;

		animationVector3Key.time = nodeAnim.mScalingKeys[i].mTime / kSecondsConversion_;

		scalings_.push_back(animationVector3Key);

		if (endTime_ < animationVector3Key.time) {
			endTime_ = animationVector3Key.time;
		}

	}

	nodeName_ = nodeAnim.mNodeName.C_Str();

}
