#pragma once
#include <cstdint>
#include <array>

const uint32_t kNumMaxInfluence = 4;

struct VertexInfluence
{
	std::array<float, kNumMaxInfluence> weights;
	std::array<int32_t, kNumMaxInfluence> indecis;
};
