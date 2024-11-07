#include "PostEffect.hlsli"

#include "PostEffectParameters.hlsli"

#include "PostEffectCalc.CS.hlsl"

ConstantBuffer<ComputeParameters> gComputeConstants : register(b0);

Texture2D<float32_t4> sourceImage0 : register(t0);

RWTexture2D<float32_t4> destinationImage0 : register(u0);

float32_t4 RadialBlur(in const float32_t2 index) {

	// 出力色
	float32_t4 output = { 0.0f,0.0f,0.0f,0.0f };

	// uv
	float32_t2 texcoord = GetTexcoord(index, float32_t2(gComputeConstants.threadIdTotalX, gComputeConstants.threadIdTotalY));

	// 中心を基準にした位置
	float32_t2 position = texcoord - gComputeConstants.radialBlurCenter;

	// 中心からの距離
	float32_t distance = length(position);
	float32_t factor = gComputeConstants.radialBlurStrength * rcp(gComputeConstants.radialBlurSamples) * distance;

	// ブラーが適用されない範囲を計算, 0.1の範囲をぼかす
	factor *= smoothstep(gComputeConstants.radialBlurMask - 0.1f, gComputeConstants.radialBlurMask, distance);

	// 新しいインデックス
	float32_t2 newIndex = float32_t2(0.0f, 0.0f);
	// サンプル分回す
	for (int32_t i = 0; i < gComputeConstants.radialBlurSamples; ++i) {
		float32_t uvOffset = 1.0f - factor * float32_t(i);
		newIndex = position * uvOffset + gComputeConstants.radialBlurCenter;
		newIndex.x *= gComputeConstants.threadIdTotalX;
		newIndex.y *= gComputeConstants.threadIdTotalY;
		output += sourceImage0[newIndex];
	}

	// 平均を求める
	output *= rcp(gComputeConstants.radialBlurSamples);
	// 出力
	return output;

}

[numthreads(THREAD_X, THREAD_Y, THREAD_Z)]
void main(uint32_t3 dispatchId : SV_DispatchThreadID) {

	if (dispatchId.x < gComputeConstants.threadIdTotalX &&
		dispatchId.y < gComputeConstants.threadIdTotalY) {

		destinationImage0[dispatchId.xy] = RadialBlur(dispatchId.xy);

	}

}