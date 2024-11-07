#include "PostEffect.hlsli"

#include "PostEffectParameters.hlsli"

#include "PostEffectCalc.CS.hlsl"

ConstantBuffer<ComputeParameters> gComputeConstants : register(b0);

Texture2D<float32_t4> sourceImage0 : register(t0);

RWTexture2D<float32_t4> destinationImage0 : register(u0);

struct ShockWaveParameters {

	float32_t2 center;
	float32_t distortion;
	float32_t radius;
	float32_t thickness;

};

ConstantBuffer<ShockWaveParameters> gShockWaveConstants0: register(b5);

float32_t4 ShockWave(in const float32_t2 index) {

	// uv
	float32_t2 texcoord = GetTexcoord(index, float32_t2(gComputeConstants.threadIdTotalX, gComputeConstants.threadIdTotalY));

	// 比率
	float32_t ratio = float32_t(gComputeConstants.threadIdTotalY) * rcp(gComputeConstants.threadIdTotalX);

	// テクスチャ比率に依存しない真円
	float32_t2 scaleUV = (texcoord - float32_t2(0.5f, 0.0f)) * float32_t2(rcp(ratio), 1.0f) + float32_t2(0.5f, 0.0f);

	// 中心を基準にした位置
	float32_t2 position = scaleUV - gShockWaveConstants0.center;

	// マスク
	float32_t mask =
		(1.0f - smoothstep(gShockWaveConstants0.radius - 0.1f, gShockWaveConstants0.radius, length(position))) *
		smoothstep(gShockWaveConstants0.radius - gShockWaveConstants0.thickness - 0.1f, gShockWaveConstants0.radius - gShockWaveConstants0.thickness, length(position));

	// 歪み
	float32_t2 distortion = normalize(position) * gShockWaveConstants0.distortion * mask;

	// 新しいインデックス
	float32_t2 newIndex = texcoord - distortion;
	newIndex.x *= gComputeConstants.threadIdTotalX;
	newIndex.y *= gComputeConstants.threadIdTotalY;

	// 出力
	return sourceImage0[newIndex];

}

[numthreads(THREAD_X, THREAD_Y, THREAD_Z)]
void main(uint32_t3 dispatchId : SV_DispatchThreadID) {

	if (dispatchId.x < gComputeConstants.threadIdTotalX &&
		dispatchId.y < gComputeConstants.threadIdTotalY) {

		destinationImage0[dispatchId.xy] = ShockWave(dispatchId.xy);

	}

}