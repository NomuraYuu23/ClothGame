#include "Bload.CS.hlsli"
#include "../RandomGenerator/RandomGenerator.hlsli"

struct Emitter {
	float32_t3 translate; // 位置
	float32_t radius; // 射出半径
	uint32_t count; // 射出数
	float32_t frequency; // 射出間隔
	float32_t frequencyTime; // 射出間隔調整時間
	uint32_t emit; // 射出許可
};

ConstantBuffer<Emitter> gEmitter : register(b0);

ConstantBuffer<PerFrame> gPerFrame : register(b1);

RWStructuredBuffer<Particle> gParticles : register(u0);

RWStructuredBuffer<int32_t> gFreeListIndex : register(u1);

RWStructuredBuffer<uint32_t> gFreeList : register(u2);

[numthreads(1, 1, 1)]
void main(uint32_t3 DTid : SV_DispatchThreadID)
{

	if (gEmitter.emit != 0) {

		RandomGenerator generator;

		generator.seed = (DTid + gPerFrame.time) * gPerFrame.time;

		for (uint32_t conutIndex = 0; conutIndex < gEmitter.count; ++conutIndex) {

			int32_t freeListIndex;

			InterlockedAdd(gFreeListIndex[0], -1, freeListIndex);

			if (0 <= freeListIndex && freeListIndex < kMaxParticles) {

				int32_t particleIndex = gFreeList[freeListIndex];

				gParticles[particleIndex].scale = float32_t3(0.5f, 0.5f, 1.0f);

				gParticles[particleIndex].translate = generator.Generate3d() * gEmitter.radius * 2.0f - gEmitter.radius + gEmitter.translate;
				gParticles[particleIndex].color.rgb = float32_t3(236.0f / 255.0f, 63.0f / 255.0f, 49.0f / 255.0f);
				gParticles[particleIndex].color.a = 1.0f;
				gParticles[particleIndex].lifeTime = 10.0f;

				float32_t velocityMax = 1.0f;

				gParticles[particleIndex].velocity =
					float32_t3(
						generator.Generate1d() * velocityMax - velocityMax * 0.5f,
						generator.Generate1d() * velocityMax - velocityMax * 0.5f,
						generator.Generate1d() * velocityMax - velocityMax * 0.5f);
				gParticles[particleIndex].currentTime = 0.0f;

			}
			else {

				InterlockedAdd(gFreeListIndex[0], 1);
				break;

			}

		}
	}

}