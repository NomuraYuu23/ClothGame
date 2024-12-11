#pragma once
#include <cstdint>
#include <array>

// 頂点が受ける影響最大数
const uint32_t kNumMaxInfluence = 4;

/// <summary>
/// 頂点が受ける影響
/// </summary>
struct VertexInfluence
{
	std::array<float, kNumMaxInfluence> weights; // 重み
	std::array<int32_t, kNumMaxInfluence> indecis; // 番号
};
