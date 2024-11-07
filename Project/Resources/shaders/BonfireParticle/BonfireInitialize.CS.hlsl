#include "Bonfire.CS.hlsli"

RWStructuredBuffer<Particle> gParticles : register(u0);

RWStructuredBuffer<int32_t> gFreeListIndex : register(u1);

RWStructuredBuffer<uint32_t> gFreeList : register(u2);

RWStructuredBuffer<float32_t> gDissolves : register(u3);

[numthreads(1024, 1, 1)]
void main(uint32_t3 DTid : SV_DispatchThreadID )
{

	uint32_t particleIndex = DTid.x;

	if (particleIndex == 0) {
		gFreeListIndex[0] = kMaxParticles - 1;
	}

	if (particleIndex < kMaxParticles) {
		gParticles[particleIndex] = (Particle)0;
		gFreeList[particleIndex] = particleIndex;
		gDissolves[particleIndex] = 1.0f;
	}

}