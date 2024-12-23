#include "JumpLanding.CS.hlsli"
#include "../../RandomGenerator/RandomGenerator.hlsli"

RWStructuredBuffer<Particle> gParticles : register(u0);

ConstantBuffer<PerFrame> gPerFrame : register(b0);

RWStructuredBuffer<int32_t> gFreeListIndex : register(u1);

RWStructuredBuffer<uint32_t> gFreeList : register(u2);

[numthreads(1024, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{

	uint32_t particleIndex = DTid.x;
	if (particleIndex < kMaxParticles) {

		if (gParticles[particleIndex].color.a != 0) {
			gParticles[particleIndex].translate += gParticles[particleIndex].velocity;
			gParticles[particleIndex].currentTime += gPerFrame.deltaTime;
			float32_t alpha = 
				1.0f - (gParticles[particleIndex].currentTime * rcp(gParticles[particleIndex].lifeTime));
			gParticles[particleIndex].color.a = saturate(alpha);

			gParticles[particleIndex].velocity.y -= 0.01f;

		}
		else {

			gParticles[particleIndex].scale = float32_t3(0.0f, 0.0f, 0.0f);
			int32_t freeListIndex;
			InterlockedAdd(gFreeListIndex[0], 1, freeListIndex);
			
			if ((freeListIndex + 1) < kMaxParticles) {
				gFreeList[freeListIndex + 1] = particleIndex;
			}
			else {
				InterlockedAdd(gFreeListIndex[0], -1, freeListIndex);
			}

		}

	}

}