#include "PostEffect.hlsli"

#include "PostEffectParameters.hlsli"

#include "PostEffectCalc.CS.hlsl"

ConstantBuffer<ComputeParameters> gComputeConstants : register(b0);

Texture2D<float32_t4> sourceImage0 : register(t0);

RWTexture2D<float32_t4> destinationImage0 : register(u0);

float32_t4 Sepia(in const float32_t4 input, in const float32_t2 index) {

	float32_t value = dot(input.rgb, float32_t3(0.2125f, 0.7154f, 0.0721f));

	float32_t3 output = lerp(input.rgb, float32_t3(value, value * 74.0f * rcp(107.0f), value * 43.0f * rcp(107.0f)), gComputeConstants.colorLerpT);

	// uv
	float32_t2 texcoord = GetTexcoord(index, float32_t2(gComputeConstants.threadIdTotalX, gComputeConstants.threadIdTotalY));

	// 距離
	float32_t2 colorLength = texcoord - gComputeConstants.colorPosition;
	colorLength.x = colorLength.x * rcp(gComputeConstants.colorSize.x);
	colorLength.y = colorLength.y * rcp(gComputeConstants.colorSize.y);

	// 補間係数
	float32_t t = 1.0f - clamp(length(colorLength), 0.0f, 1.0f);

	// 出力
	output.rgb = lerp(input.rgb, output, t);

	return float32_t4(output, input.a);

}

[numthreads(THREAD_X, THREAD_Y, THREAD_Z)]
void main(uint32_t3 dispatchId : SV_DispatchThreadID) {

	if (dispatchId.x < gComputeConstants.threadIdTotalX &&
		dispatchId.y < gComputeConstants.threadIdTotalY) {

		float32_t4 input = sourceImage0[dispatchId.xy];
		destinationImage0[dispatchId.xy] = Sepia(input, dispatchId.xy);

	}

}