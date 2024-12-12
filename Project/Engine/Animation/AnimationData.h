#pragma once
#include "NodeAnimationData.h"

/// <summary>
/// アニメーションデータ
/// </summary>
struct AnimationData
{

	// ノードアニメーション
	std::vector<NodeAnimationData> nodeAnimationDatas_;
	// ノードアニメーション数
	uint32_t nodeAnimationNum_;
	// 終了時間
	double endTime_;

};

